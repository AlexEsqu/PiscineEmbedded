#include "libalex_avr.h"


int	isDigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	isHexDigit(char c)
{
	if (c >= '0' && c <= '9')
		return 1;

	if (c >= 'a' && c <= 'f')
		return 1;

	if (c >= 'A' && c <= 'F')
		return 1;

	return 0;
}

uint32_t	ft_atou(const char *str)
{
	uint32_t	result;

	result = 0;
	while (*str == ' ' || *str == '\t' || *str == '\n'
		|| *str == '\r' || *str == '\f' || *str == '\v')
		str++;

	while (isDigit(*str))
		result = result * 10 + (*str++ - '0');
	return (result);
}

int32_t	ft_atoi(const char *str)
{
	int	i;
	int	minus;
	int32_t	number;

	i = 0;
	minus = -1;
	number = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			minus = minus * (-1);
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		number = number * 10 - (str[i] - '0');
		i++;
	}
	return (number * minus);
}

int	ft_strcmp(char* str1, char* str2)
{
	while (*str1 && *str2 && *str1 == *str2)
	{
		str1++;
		str2++;
	}
	return (*str1 - *str2);
}

int	bufferHasSpace(int bufferIndex)
{
	if (bufferIndex > BUFFER_SIZE - 1)
		return 0;
	return 1;
}

void	*ft_memset(void *ptr, int value, unsigned long len)
{
	unsigned char	*str;

	str = ptr;
	while (len--)
		*str++ = (unsigned char)value;
	return (ptr);
}


void handleUserTyping(char* buffer, int* bufferIndex, e_state* state)
{
	if (!(UCSR0A & (1 << RXC0)))
		return;

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
			uart_printstr("\r\n");
			*state = EXECUTE_COMMAND;
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
