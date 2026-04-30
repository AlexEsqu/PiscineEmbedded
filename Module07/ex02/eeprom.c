#include "libalex_avr.h"

unsigned char EEPROM_read(unsigned int uiAddress)
{
	if (uiAddress >= EEPROM_SIZE)
        return 0xFF;

	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE))
		;

	/* Set up address register */
	EEAR = uiAddress;

	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);

	/* Return data from Data Register */
	return EEDR;
}

void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
	if (uiAddress > EEPROM_SIZE)
		return;

	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE))
		;

	/* Set up address and Data Registers */
	EEAR = uiAddress;
	EEDR = ucData;

	/* Write logical one to EEMPE */
	EECR |= (1<<EEMPE);

	/* Start eeprom write by setting EEPE */
	EECR |= (1<<EEPE);
}

void printAddr(uint32_t num)
{
	const char hex[] = "0123456789abcdef";
	char buf[7];
	uint32_t i = 0;

	while (num > 0 && i < sizeof(buf))
	{
		buf[i++] = hex[num % 16];
		num /= 16;
	}

	while (i < sizeof(buf))
	{
		buf[i++] = '0';
	}

	while (i > 0)
	{
		uart_tx(buf[--i]);
	}
}

void printByte(unsigned char c)
{
	const char hex[] = "0123456789abcdef";
	char buf[2];
	long i = 0;

	if (c == 0)
	{
		uart_tx('0');
		uart_tx('0');
		return;
	}

	if (c < 16)
		uart_tx('0');

	while (c > 0 && i < sizeof(buf))
	{
		buf[i++] = hex[c % 16];
		c /= 16;
	}

	while (i > 0)
	{
		uart_tx(buf[--i]);
	}
}

void	hexdumpEEPROM()
{
	for (unsigned int address = 0; address < EEPROM_SIZE; address += 16)
	{
		printAddr(address);
		uart_printstr(": ");
		char buffer[16];
		for (unsigned int i = 0; i < 16; i++)
		{
			int byte = address + i;
			buffer[i] = EEPROM_read(byte);
			printByte(buffer[i]);
			uart_tx(' ');
		}

		uart_printstr("|");
		for (unsigned int i = 0; i < 16; i++)
		{
			uart_tx(buffer[i]);
		}
		uart_printstr("|");

		uart_printstr("\r\n");
	}
}


void	hexdumpEEPROMWithModif(eeprom_balaylaka_t result)
{
	long sixteenLinesBeforeModif = result.byteAddress - (16 * 6);
	if (sixteenLinesBeforeModif < 0)
		sixteenLinesBeforeModif = 0;

	uint32_t sixteenLinesAfterModif = result.byteAddress + (16 * 6);
	if (sixteenLinesAfterModif > EEPROM_SIZE)
		sixteenLinesBeforeModif = EEPROM_SIZE;

	for (unsigned int address = sixteenLinesBeforeModif; address <  sixteenLinesAfterModif; address += 16)
	// for (unsigned int address = 0; address < EEPROM_SIZE; address += 16)
	{
		printAddr(address);
		uart_printstr(": ");
		char buffer[16];

		for (unsigned int i = 0; i < 16; i++)
		{
			unsigned int byte = address + i;

			if (byte == result.byteAddress)
				uart_printstr("\e[31m");

			buffer[i] = EEPROM_read(byte);
			printByte(buffer[i]);

			if (byte == result.byteAddress)
				uart_printstr("\e[0m");

			uart_tx(' ');
		}

		uart_printstr("|");
		for (unsigned int i = 0; i < 16; i++)
		{
			uart_tx(buffer[i]);
		}
		uart_printstr("|");

		uart_printstr("\r\n");
	}
}


void	hexdumpEEPROMAroundAddress(uint16_t addr)
{
	long sixteenLinesBeforeModif = addr - (16 * 6);
    if (sixteenLinesBeforeModif < 0)
        sixteenLinesBeforeModif = 0;

    uint32_t sixteenLinesAfterModif = addr + (16 * 6);
    if (sixteenLinesAfterModif > EEPROM_SIZE)
        sixteenLinesAfterModif = EEPROM_SIZE;

    for (unsigned int address = sixteenLinesBeforeModif; address <  sixteenLinesAfterModif; address += 16)
    {
		printAddr(address);
		uart_printstr(": ");
		char buffer[16];

		for (unsigned int i = 0; i < 16; i++)
		{
			unsigned int byte = address + i;

			if (byte == addr)
				uart_printstr("\e[31m");

			buffer[i] = EEPROM_read(byte);
			printByte(buffer[i]);

			if (byte == addr)
				uart_printstr("\e[0m");

			uart_tx(' ');
		}

		uart_printstr("|");
		for (unsigned int i = 0; i < 16; i++)
		{
			uart_tx(buffer[i]);
		}
		uart_printstr("|");

		uart_printstr("\r\n");
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




eeprom_balaylaka_t	extractAddressAndNewValue(char* buffer)
{
	char				address[4];
	char				newByte[4];
	int					i = 0;
	int					j = 0;
	eeprom_balaylaka_t	result;

	while (isHexDigit(buffer[i]))
	{
		address[i] = buffer[i];
		i++;
	}

	address[i] = 0;
	i++;

	j = 0;
	while (isHexDigit(buffer[i]))
	{
		newByte[j] = buffer[i];
		i++;
		j++;
	}

	newByte[j] = 0;

	result.byteAddress = atoiHex(address);
	result.newValue = atoiHex(newByte);
	return result;
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