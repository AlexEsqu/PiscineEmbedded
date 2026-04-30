#include "libalex_avr.h"

// Write a command-line program on the UART.
// When given a valid address on the EEPROM in hexadecimal it writes an hexadecimal
// value in a byte at the address to the EEPROM, then displays the EEPROM contents
// with the byte in red.
// If the byte already had this value, nothing should happen.

# define BUFFER_SIZE 10

typedef enum
{
	PROMPT_ADDRESS,
	RECEIVE_ADDRESS,
	ADDRESS_VALIDATION,
	WRONG_ADDRESS,
	UPDATED_HEXDUMP
} e_state;

void	bzeroStr(char *str, int size)
{
	for (int i = 0; i < size; i++)
	{
		str[i] = 0;
	}
}

char bufferHasSpace(int bufferIndex)
{
	if (bufferIndex >= BUFFER_SIZE - 1)
		return 0;
	return 1;
}

void handleUserTyping(char* buffer, int* bufferIndex, e_state* state)
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
			uart_printstr("\r\n");
			*state = ADDRESS_VALIDATION;
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

uint32_t atoiHex(const char *str)
{
	uint32_t num = 0;
	uint8_t	index = 0;

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

	if (buffer[2] != ' ' && buffer[3] != ' ' && buffer[4] != ' ')
		return 0;

	return 1;
}

eeprom_balaylaka_t	extractAddressAndNewValue(char* buffer)
{
	char				address[6];
	char				newByte[3];
	int					i = 0;
	int					j = 0;
	eeprom_balaylaka_t	result;

	while (isHexDigit(buffer[i]) && i < 5)
	{
		address[i] = buffer[i];
		i++;
	}
	address[i] = '\0';

	// skip the space
	i++;

	j = 0;
	while (isHexDigit(buffer[i]) && j <= 2)
	{
		newByte[j] = buffer[i];
		i++;
		j++;
	}
	newByte[j] = '\0';

	result.byteAddress = atoiHex(address);
	result.newValue = atoiHex(newByte);
	return result;
}


int main()
{
	uart_init();

	char	buffer[BUFFER_SIZE];
	int		bufferIndex = 0;
	e_state	state = PROMPT_ADDRESS;
	eeprom_balaylaka_t	result = {0};

	while (1)
	{
		switch (state)
		{
			case PROMPT_ADDRESS:
			{
				uart_printstr("Please input an address byte, and a value for it:\r\n> ");
				bzeroStr(buffer, BUFFER_SIZE);
				bufferIndex = 0;

				state = RECEIVE_ADDRESS;
				result.newValue = 0;
				result.byteAddress = 0;
				break;
			}
			case RECEIVE_ADDRESS:
			{
				handleUserTyping(buffer, &bufferIndex, &state);
				break;
			}
			case ADDRESS_VALIDATION:
			{
				if (!isValidFormat(buffer))
				{
					state = WRONG_ADDRESS;
					break;
				}
				result = extractAddressAndNewValue(buffer);
				if (result.byteAddress >= EEPROM_SIZE)
					state = WRONG_ADDRESS;
				else if (result.newValue >= 256)
					state = WRONG_ADDRESS;
				else
					state = UPDATED_HEXDUMP;
				break;
			}
			case WRONG_ADDRESS:
			{
				uart_printstr("Wrong address format, please try again.\r\n");
				state = PROMPT_ADDRESS;
				break;
			}
			case UPDATED_HEXDUMP:
			{
				uart_printstr("You have selected: [");
				uart_printhex(result.newValue);
				uart_printstr("] to be written at address ");
				uart_printhex(result.byteAddress);
				uart_printstr("\r\n");
				if (EEPROM_read(result.byteAddress) != result.newValue)
					EEPROM_write(result.byteAddress, result.newValue);
				hexdumpEEPROMWithModif(result);
				state = PROMPT_ADDRESS;
				break;
			}
		}
	}
}



// EEARH =  EEPROM Address Register High
// 	–			–			–			–			–			–			EEAR9		EEAR8
// [																	]	[EEPROM address			]

// EEARH =  EEPROM Address Register Low
//	EEAR7		EEAR6		EEAR5		EEAR4		EEAR3		EEAR2		EEAR1		EEAR0
// [	EEPROM address																				]

// EEDR – The EEPROM Data Register
// MSB			-			-			-			-			-			-			LSB
// [	Data to be written / Data read out															]


// EECR – The EEPROM Control Registe
// –			–			EEPM1		EEPM0		EERIE		EEMPE		EEPE		EERE
// [					  ]	[Programming Mode	  ]	[Interrupt]	[MasterWri]	[WriteEnab]	[ReadEnabl]
