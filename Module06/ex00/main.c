#include "libalex_avr.h"

// • Le microcontrôleur AVR ATmega328P possède 1 interface I2C que vous devez uti-
// liser dans cet exercice pour communiquer avec un capteur de température AHT20
// (U3).
// • Vous devez écrire une fonction i2c_init qui initialise l’I2C sur le microcontrôleur.
// • L’I2C du MCU doit être configuré afin que la fréquence de communication soit de
// 100 kHz.
// • Vous devez écrire une fonction i2c_start qui démarre une transmission I2C entre
// le microcontrôleur et le capteur.
// • Le programme devra retourner sur votre ordinateur les valeurs de statut après
// chaque envoi de donnée.
// • Vous devez écrire une fonction i2c_stop qui interrompt la communication entre
// le microcontrôleur et le capteur.

void i2c_init(void)
{

}

// per datasheet p.225
void i2c_start(void)
{
	// send START condition to take control of the TWI bus
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	// wait for the START to be transmitted
	while (!(TWCR & (1<<TWINT)))
		;

	// check the start has been accepted
	if ((TWSR & 0xF8) != START)
		uart_printstr("Start has been denied.\r\n");
}

void i2c_stop(void)
{

}

int main()
{
	uart_init();
	i2c_init();
}

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
