#include "libalex_avr.h"


int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

uint32_t	ft_atou(const char *str)
{
	uint32_t	result;

	result = 0;
	while (*str == ' ' || *str == '\t' || *str == '\n'
		|| *str == '\r' || *str == '\f' || *str == '\v')
		str++;

	while (ft_isdigit(*str))
		result = result * 10 + (*str++ - '0');
	return (result);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	minus;
	int	number;

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

uint32_t ft_strlen(char* str)
{
	uint32_t len = 0;
	while (str[len++])
		;
	return len;
}

void	*ft_memmove(void *dest, const void *src, uint16_t len)
{
	uint32_t			i;
	unsigned char	*strsrc;
	unsigned char	*strdest;

	i = 0;
	strsrc = (unsigned char *)src;
	strdest = (unsigned char *)dest;
	if (dest > src)
	{
		while (len > 0)
		{
			len--;
			strdest[len] = strsrc[len];
		}
	}
	else if (dest < src)
	{
		while (i < len)
		{
			strdest[i] = strsrc[i];
			i++;
		}
	}
	return (dest);
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

void	*ft_memset(void *ptr, int value, unsigned long len)
{
	unsigned char	*str;

	str = ptr;
	while (len--)
		*str++ = (unsigned char)value;
	return (ptr);
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