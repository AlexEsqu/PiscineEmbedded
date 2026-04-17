#include "libalex_avr.h"

// You must make a program that listen to the serial port.
// When you send a new line with an HEX RGB color with the format #RRGGBB sets the
// color on the RGB LED on pin D5.

# define BUFFER_SIZE 8

bool bufferHasSpace(int bufferIndex)
{
	return (bufferIndex < BUFFER_SIZE - 1);
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
	for (int i = 1; i <= 6; i++)
	{
		if (!(str[i] >= '0' && str[i] <= '9')
			&& !(str[i] >= 'A' && str[i] <= 'F')
			&& !(str[i] >= 'a' && str[i] <= 'f'))
			return false;
	}
	return true;
}

uint32_t atoiHex(const char *str)
{
    uint32_t num = 0;
    int index = (str[0] == '#') ? 1 : 0;

    while (str[index])
    {
		uint8_t digit;
        if (str[index] >= '0' && str[index] <= '9')
            digit = (uint8_t)(str[index] - '0');
        else if (str[index] >= 'A' && str[index] <= 'F')
            digit = (uint8_t)(str[index] - 'A' + 10);
        else if (str[index] >= 'a' && str[index] <= 'f')
            digit = (uint8_t)(str[index] - 'a' + 10);
        else
            break;

        num = num * 16 + digit;
		index++;
    }
    return num;
}


void handleUserTyping(char* buffer, int* bufferIndex)
{
	char currentChar = uart_rx();

	while (currentChar != '\r')
	{
		switch (currentChar)
		{
			case '\x7F': // allow for back key
			{
				if (*bufferIndex <= 0)
					break;
				buffer[*bufferIndex] = '\0';
				(*bufferIndex)--;
				uart_printstr("\b \b");
				break;
			}
			case '\x1b': // escape any control character to limit the madness
			{
				(void)uart_rx(); // [ character
				for (char c = uart_rx(); c >= '0' && c <= '9' && c == ';';)
					;
				break;
			}
			default: // store chars in buffer
			{
				if (bufferHasSpace(*bufferIndex) && currentChar >= ' ')
				{
					buffer[*bufferIndex] = currentChar;
					(*bufferIndex)++;
					uart_tx(currentChar);
				}
			}
		}
		currentChar = uart_rx();
	}
	uart_printstr("\r\n");
}

uint32_t	promptForColor()
{
	char	buffer[BUFFER_SIZE];
	emptyBuffer(buffer, BUFFER_SIZE);
	int		bufferIndex = 0;
	
	while (!isValidColorCode(buffer))
	{
		uart_printstr("Please input a valid color hexcode: ");
		emptyBuffer(buffer, BUFFER_SIZE);
		bufferIndex = 0;
		handleUserTyping(buffer, &bufferIndex);
	}

	return (atoiHex(buffer));
}

int main()
{
	uart_init();
	uart_printstr("Hello and welcome to my microcontroller !\r\n");

	while (1)
	{
		// store in 32 cuz color is big and int are smol here
		uint32_t color = promptForColor();

		uart_printstr("\r\nYou have requested : ");
		uart_printhex(color);
		uart_printstr("\r\nPlease, BEHOLD THE LED\r\n\r\n\r\n");
		
		init_rgb();
		set_rgb(get_red(color), get_green(color), get_blue(color));
	}
}
