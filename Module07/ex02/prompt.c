#include "libalex_avr.h"


void	bzeroStr(char *str, int size)
{
	for (int i = 0; i < size; i++)
	{
		str[i] = 0;
	}
}

char bufferHasSpace(int bufferIndex)
{
	if (bufferIndex > BUFFER_SIZE - 1)
		return 0;
	return 1;
}

int ft_strcmp(char* str1, char* str2)
{
	while (*str1 && *str1++ == *str2++)
		;
	return (*str1 - *str2);
}


char	isHexDigit(char c)
{
	if (c >= '0' && c <= '9')
		return 1;

	if (c >= 'a' && c <= 'f')
		return 1;

	if (c >= 'A' && c <= 'F')
		return 1;

	return 0;
}

char	isValidFormat(char* buffer)
{
	for (int i = 0; i < BUFFER_SIZE && buffer[i]; i++)
	{
		if (!isHexDigit(buffer[i]) && buffer[i] != ' ')
			return 0;
	}

	if (buffer[2] != ' ' && buffer[3] != ' ')
		return 0;

	return 1;
}


void handleUserTyping(char* buffer, int* bufferIndex, e_state* state)
{
	char currentChar = uart_rx();
	(void)state;

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
			uart_printstr("\r\n");
			*state = VALIDATE_EXECUTE;
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
				uart_tx(currentChar);
			}
		}
	}
}