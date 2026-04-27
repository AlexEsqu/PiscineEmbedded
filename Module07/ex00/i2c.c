#include "libalex_avr.h"

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

void	waitForI2cTransmission()
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

	TWSR = 0;
	// leaving prescaler at 1 so not changing TWPS1 and TWPS0
}

// per datasheet p.225
void i2c_start(void)
{
	// uart_printstr("Status before transmission of START:");
	// uart_printhex(getI2cStatusCode());

	// send START condition to take control of the TWI bus
	// see datasheet p.226
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	waitForI2cTransmission();

	// uart_printstr("\r\nStatus after transmission of START:");
	// uart_printhex(getI2cStatusCode());
}

// per datasheet p.225
void i2c_renew_start(void)
{
	// uart_printstr("Status before transmission of RENEW START:");
	// uart_printhex(getI2cStatusCode());

	// send START condition to take control of the TWI bus
	// see datasheet p.226
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	waitForI2cTransmission();

	// uart_printstr("\r\nStatus after transmission of RENEW START:");
	// uart_printhex(getI2cStatusCode());
}

void	i2c_enter_master_receiver()
{
	// load SLA+R into the register to enter Master Receiver mode
	// see datasheet p.227
	TWDR = ((SENSOR_ADDRESS << 1) | 1);

	// Send SLA+R mode
	TWCR = (1<<TWINT) | (1<<TWEN);
	TWCR &= ~((1<<TWSTO) | (1<<TWSTA));

	waitForI2cTransmission();

	// uart_printstr("\r\nStatus after transmission of SLA+W:");
	// uart_printhex(getI2cStatusCode());
}

void	i2c_enter_master_transmitter()
{
	// load SLA+W into the register to enter Master Transmitter mode
	// see datasheet p.229
	TWDR = ((SENSOR_ADDRESS << 1) | 0);

	// Send SLA+W mode
	TWCR = (1<<TWINT) | (1<<TWEN);
	TWCR &= ~((1<<TWSTO) | (1<<TWSTA));

	waitForI2cTransmission();

	// uart_printstr("\r\nStatus after transmission of SLA+R:");
	// uart_printhex(getI2cStatusCode());
}

void	i2c_write(unsigned char data)
{
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);

	waitForI2cTransmission();

	// uart_printstr("\r\nStatus after transmission of DATA:");
	// uart_printhex(getI2cStatusCode());
}

char	i2c_read(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1 << TWEA);
	waitForI2cTransmission();
	char data = TWDR;

	// uart_printstr("\r\nStatus after transmission of DATA:");
	// uart_printhex(getI2cStatusCode());


	return (data);
}

void i2c_stop(void)
{
	// transmit STOP, end transmission
	TWCR |= (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}
