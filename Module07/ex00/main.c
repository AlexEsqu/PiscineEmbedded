#include "libalex_avr.h"

// Write a program that displays the status of the entire EEPROM on the UART in
// hexdump format.
// 00000000 57 68 61 74 20

int main()
{
	
}






//
// TWCR - TWI Control Register
// 	TWINT		TWEA		TWSTA		TWSTO		TWWC		TWEN		----		TWIE
//	[finished]	[ack enabl]	[ START ]	[ STOP ]	[wr collis]	[TWI enabl]	[		]	[TWI interrupt]

// TWSR - TWI Status Register
// 	TWS7		TWS6		TWS5		TWS4		TWS3		-----		TWPS1		TWPS0
//	[TWI logic												]	[		]	[ prescaler				]

// TWDR - TWI Data Register
// 	TWD7		TWD6		TWD5		TWD4		TWD3		TWD2		TWD1		TWD0
//	[	DATA to be transmitted (Transmit mode) or received (Receive mode)							]

// TWBR - TWI Bit Rate Register
// 	TWBR7		TWBR6		TWBR5		TWBR4		TWBR3		TWBR2		TWBR1		TWBR0
//	[ Division factor for the bit rate generator													]

// TWAMR – TWI (Slave) Address Mask Register
// 	TWAM7		TWAM6		TWAM5		TWAM4		TWAM3		TWAM2		TWAM1		TWAM0
//	[ Address of target slave																		]
