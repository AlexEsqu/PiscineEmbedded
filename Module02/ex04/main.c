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
	if (bufferIndex > BUFFER_SIZE - 1)
		return false;
	return true;
}

void uart_hex(uint32_t x) {
	const char *hex = "0123456789ABCDEF";
	char buf[sizeof(uint32_t) * 2 + 1];

	buf[sizeof(uint32_t) * 2] = '\0';
	for (unsigned int i = 0; i < sizeof(uint32_t) * 2; i++) {
		buf[sizeof(uint32_t) * 2 - i - 1] = hex[(x & 0xf)];
		x >>= 4;
	}
	uart_printstr(buf);
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
			for (char c = uart_rx(); c >= '0' && c <= '9' && c == ';';)
				;
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

// hashing function, completely unnecessary, but I promised Serghei
uint32_t fnv1a(const uint8_t *data) {
    uint32_t hash = 0x811C9DC5; // offset basis

    for (uint32_t i = 0; data[i]; i++) {
        hash ^= data[i];
        hash *= 0x01000193; // FNV prime
    }

    return hash;
}

void __attribute__((signal)) __vector_11 (void)
{
	PORTB ^= 0b00010111;
}

void	makeBlink()
{
	OCR1A = F_CPU / (2 * 256 * 1) - 1;
	
	timer_init(TIMER_MODE_CTC, TOP_OCR1A, CMP_SET, CMP_DISCONNECT);
	SREG |= (1 << 7);
	TIMSK1 |= (1 << OCIE1A);
	TCCR1B |= 0b00000101;
}

int main()
{
	uart_init();
	e_state	state = PROMPT_LOGIN;
	char	buffer[BUFFER_SIZE];
	int		bufferIndex = 0;
	bzeroStr(buffer, BUFFER_SIZE);
	char*	login = "mkling";
	uint32_t pwd = 0x8E637645; // 

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
				bufferIndex = 0;
				break;
			}
			case PROMPT_PWD:
			{
				uart_printstr("\t\tPassword:");
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
				if (fnv1a((uint8_t *)buffer) == pwd)
				{
					state = CORRECT;
					uart_printstr("\r\n\r\n\t\t<Welcome Text>\r\n");
					makeBlink();
				}
				else
					state = WRONG;
				bzeroStr(buffer, BUFFER_SIZE);
				bufferIndex = 0;
				break;
			}
			case WRONG:
			{
				uart_printstr("\r\n\r\n\t\tBOOOH, WRONG!!\r\n");
				state = PROMPT_LOGIN;
				break;
			}
			case CORRECT:
			{
				break;
			}
		}
	}
}
