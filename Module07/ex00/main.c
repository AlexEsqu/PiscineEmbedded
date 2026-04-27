#include "libalex_avr.h"

// Write a program that displays the status of the entire EEPROM on the UART in
// hexdump format.
// 00000000 57 68 61 74 20

#define EEPROM_SIZE 1023


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


int main()
{
	uart_init();
	uart_printstr("Beep:");
	for (int address = 0x0; address < EEPROM_SIZE; address++)
	{
		uart_utoa(address);
		uart_tx(' ');
		for (int byte = address + 0; byte < 8; byte++)
			uart_printhex(EEPROM_read(address));
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
