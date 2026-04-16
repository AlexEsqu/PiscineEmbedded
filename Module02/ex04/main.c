#include "libalex_avr.h"

// • Create 2 strings, a username and a password.
// • Display a prompt on the serial port that asks for the username and password.
// • When typing the username, there should be an echo.
// • same for the password but with ’*’ instead of characters.
// • The Backspace key deletes a character.
// • The Enter key validates the input.
// • If the username and password are correct, the program displays the welcome text
//   and makes the LEDs blink.
//     Bonus points if you add a dramatic effect. ;)
// • Otherwise, the program displays the error message.

# define BUFFER_SIZE 20

typedef enum
{
	PROMPT_LOGIN,
	AWAIT_LOGIN,
	VERIF_LOGIN,
	PROMPT_PWD,
	AWAIT_PWD,
	VERIF_PWD,
	WRONG,
	CORRECT
} e_state;

typedef enum
{
	LOGIN,
	PWD
} e_content;

bool bufferHasSpace(int bufferIndex)
{
	if (bufferIndex >= BUFFER_SIZE)
		return false;
	return true;
}

void uart_hex(char c) {
	const char *hex = "0123456789ABCDEF";
	int hi = c >> 4;
	int lo = c & 0xf;

	uart_tx(hex[hi]);
	uart_tx(hex[lo]);
	uart_tx('\n');
}

void handleUserTyping(char* buffer, int* bufferIndex, e_content mode, e_state* state)
{
	char currentChar = uart_rx();

	switch (currentChar)
	{
		case '\x7F':
		{
			if (*bufferIndex <= 0)
				break;
			buffer[*bufferIndex] = '\0';
			(*bufferIndex)--;
			uart_printstr("\b \b");
			break;
		}
		case '\r':
		{
			if (mode == LOGIN)
				*state = VERIF_LOGIN;
			else
				*state = VERIF_PWD;
			uart_printstr("\r\n");
			break;
		}
		case '\x1b':
		{
			(void)uart_rx(); // [ character
			for (char c = uart_rx(); c >= '0' && c <= '9' && c == ';';);
			break;
		}
		default:
		{
			if (bufferHasSpace(*bufferIndex) && currentChar >= ' ')
			{
				buffer[*bufferIndex] = currentChar;
				(*bufferIndex)++;
				if (mode == LOGIN)
					uart_tx(currentChar);
				else
					uart_tx('*');
			}
		}
	}
}

bool isIdentical(char* str1, char* str2)
{
	while (*str1 && *str1++ == *str2++)
		;
	return (*str1 == *str2);
}

void	bzeroStr(char *str, int strSize)
{
	for (int i = 0; i < strSize; i++)
	{
		str[i] = '\0';
	}
}

int main()
{
	uart_init();
	e_state	state = PROMPT_LOGIN;
	char	buffer[BUFFER_SIZE];
	int		bufferIndex = 0;
	char*	login = "mkling";
	char*	pwd = "superSecure";

	bzeroStr(buffer, BUFFER_SIZE);

	while (1)
	{
		switch (state)
		{
			case PROMPT_LOGIN:
			{
				uart_printstr("Hello and welcome to my microcontroller !\r\n");
				uart_printstr("Please input your login:\r\n");
				uart_printstr("\t\tLogin:");
				state = AWAIT_LOGIN;
				break;
			}
			case AWAIT_LOGIN:
			{
				handleUserTyping(buffer, &bufferIndex, LOGIN, &state);
				break;
			}
			case VERIF_LOGIN:
			{
				if (isIdentical(buffer, login))
					state = PROMPT_PWD;
				else
					state = WRONG;
				bzeroStr(buffer, BUFFER_SIZE);
				break;
			}
			case PROMPT_PWD:
			{
				uart_printstr("\t\tPassword:");
				bufferIndex = 0;
				bzeroStr(buffer, BUFFER_SIZE);
				state = AWAIT_PWD;
				break;
			}
			case AWAIT_PWD:
			{
				handleUserTyping(buffer, &bufferIndex, PWD, &state);
				break;
			}
			case VERIF_PWD:
			{
				uart_printstr(buffer);
				if (isIdentical(buffer, pwd))
					state = CORRECT;
				else
					state = WRONG;
				break;
			}
			case WRONG:
			{
				uart_printstr("\r\n\r\n\t\tBOOOH, WRONG PASSWORD!!\r\n");
				state = PROMPT_LOGIN;
				break;
			}
			case CORRECT:
			{
				uart_printstr("\r\n\r\n\t\tCorrect!!!\r\n");
				state = PROMPT_LOGIN;
				break;
			}
		}
	}
}
