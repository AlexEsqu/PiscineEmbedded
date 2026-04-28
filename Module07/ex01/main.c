#include "libalex_avr.h"

// Write a command-line program on the UART.
// When given a valid address on the EEPROM in hexadecimal it writes an hexadecimal
// value in a byte at the address to the EEPROM, then displays the EEPROM contents
// with the byte in red.
// If the byte already had this value, nothing should happen.


int main(int argc, char** argv)
{
	uart_init();
	
	

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
