#include "libalex_avr.h"

// Write a program that displays the status of the entire EEPROM on the UART in
// hexdump format.
// 00000000 57 68 61 74 20

#define EEPROM_SIZE 1024


unsigned char EEPROM_read(unsigned int uiAddress)
{
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


int main()
{
	uart_init();
	for (unsigned int address = 0; address < EEPROM_SIZE; address += 16)
    {
		printAddr(address);
		uart_printstr(": ");
		char buffer[16];
		for (unsigned int i = 0; i < 16; i++)
		{
			int byte = address + i;
			buffer[i] = EEPROM_read(byte);
			uart_printhex(buffer[i]);
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
