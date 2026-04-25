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

// Per datasheet of the AHT20 module (https://datasheet4u.com/pdf/1551700/AHT20.pdf)
// Address of the temperature captor is:
#define SENSOR_ADDRESS 0x38

// Per datasheet p.227-230, i2C status codes are:
typedef enum
{
	I2C_START_TRANSMITTED	= 0x08,
	I2C_START_REPEATED		= 0x10,
	I2C_MT_SLA_W_ACK		= 0x18,
	I2C_MT_SLA_W_NOT_ACK	= 0x20,
	I2C_MT_DATA_ACK			= 0x28,
	I2C_MT_DATA_NOT_ACK		= 0x30,
	I2C_ARBITRATION_LOST	= 0x38,
	I2C_MR_SLA_R_ACK		= 0x40,
	I2C_MR_SLA_R_NOT_ACK	= 0x48,
	I2C_MR_DATA_ACK			= 0x50,
	I2C_MR_DATA_NOT_ACK		= 0x58
} e_i2c_mt_status_code;


uint8_t	getI2cStatusCode()
{
	// shift 2 prescaler bits and 1 reserved bits
	// leave only the status code
	return (TWSR & 0xF8);
}

void	waitForTransmission()
{
	while (!(TWCR & (1<<TWINT)))
		;
}

void i2c_init(void)
{
	// per datasheet p.222 : SCL Frequency F_SCL = (F_CPU) / (16 + 2(TWBR) * Prescaler)
	// which reduces to TWBR = ((CPU Clock frequency / SCL frequency) – 16) / 2
	// source: https://www.arxterra.com/lecture-9-serial-communications-and-i2c/
	// not my own math that would be madness
	TWBR = (((float)F_CPU / (float)100000) - 16) / 2;

	// per datasheet, inputting address of the slave, here the temperature and humidity module
	// per datasheet of the module p.12, that address is 0x38

	TWSR = 0;
	// leaving prescaler at 1 so not changing TWPS1 and TWPS0
}


// per datasheet p.225
void i2c_start(void)
{
	uart_printstr("Status before transmission of START:");
	uart_printhex(getI2cStatusCode());

	// send START condition to take control of the TWI bus
	// see datasheet p.226
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	waitForTransmission();

	uart_printstr("\r\nStatus after transmission of START:");
	uart_printhex(getI2cStatusCode());

	// load SLA+W into the register to enter Master mode
	// see datasheet p.227
	TWDR = ((SENSOR_ADDRESS << 1) | 0);

	// Sending transfer
	TWCR = (1<<TWINT) | (1<<TWEN);
	TWCR &= ~((1<<TWSTO) | (1<<TWSTA));

	waitForTransmission();

	uart_printstr("\r\nStatus after transmission of SLA+W:");
	uart_printhex(getI2cStatusCode());

}

void	i2c_send(void)
{
	TWDR = 0x00;
	TWCR = (1<<TWINT) | (1<<TWEN);

	waitForTransmission();

	uart_printstr("\r\nStatus after transmission of DATA:");
	uart_printhex(getI2cStatusCode());
	if ((TWSR & 0xF8) != I2C_MT_DATA_ACK)
	{
		uart_printstr("Data not acknowledged.\r\n");
	}
}

void i2c_stop(void)
{
	// transmit STOP, end transmission
	TWCR &= ~(1<<TWEA);
	TWCR |= (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

	uart_printstr("\r\nStatus after transmission of STOP:");
	uart_printhex(getI2cStatusCode());
}

int main()
{
	uart_init();
	i2c_init();

	i2c_start();
	// i2c_send();
	i2c_stop();
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
