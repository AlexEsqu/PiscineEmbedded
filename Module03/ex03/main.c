#include "libalex_avr.h"

// You must make a program that listen to the serial port.
// When you send a new line with an HEX RGB color with the format #RRGGBB sets the
// color on the RGB LED on pin D5.

# define BUFFER_SIZE 8

bool bufferHasSpace(int bufferIndex)
{
	if (bufferIndex > BUFFER_SIZE - 1)
		return false;
	return true;
}

void	emptyBuffer(char *str, int strSize)
{
	for (int i = 0; i < strSize; i++)
	{
		str[i] = '\0';
	}
}

bool	isValidColorCode(char* str)
{
	if (str[0] != '#')
		return false;
	for (int i = 1; i < 6; i++)
	{
		if (!(str[i] >= '0' && str[i] <= '9')
			&& !(str[i] >= 'A' && str[i] <= 'F'))
			return false;
	}
	return true;
}

int		atoiHex(char* str)
{
	int	num = 0;
	
	while (*str)
	{
		if (*str)
	}
}

void handleUserTyping(char* buffer, int* bufferIndex)
{
	char currentChar = uart_rx();

	while (currentChar != '\r')
	{
		switch (currentChar)
		{
			// allow for back key
			case '\x7F':
			{
				if (*bufferIndex <= 0)
					break;
				buffer[*bufferIndex] = '\0';
				(*bufferIndex)--;
				uart_printstr("\b \b");
				break;
			}
			// escape any control character to limit the madness
			case '\x1b':
			{
				(void)uart_rx(); // [ character
				for (char c = uart_rx(); c >= '0' && c <= '9' && c == ';';)
					;
				break;
			}
			// store chars in buffer
			default:
			{
				if (bufferHasSpace(*bufferIndex) && currentChar >= ' ')
				{
					buffer[*bufferIndex] = currentChar;
					(*bufferIndex)++;
					uart_tx(currentChar);
				}
			}
		}
	}
	uart_printstr("\r\n");
}

int	promptForColor()
{
	char	buffer[BUFFER_SIZE];

	emptyBuffer(buffer, BUFFER_SIZE);
	uart_printstr("Hello and welcome to my microcontroller !\r\n");

	while (!isValidColorCode(buffer))
	{
		uart_printstr("Please input a valid color hexcode: ");
		handleUserTyping();
	}

	uart_printstr("\r\nYou have requested : ");
	uart_printstr(buffer);
	return (atoiHex(buffer));
}

int main()
{
	uart_init();

	while (1)
	{
		int color = promptForColor();
	}
}
