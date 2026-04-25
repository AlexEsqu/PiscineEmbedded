#include "libalex_avr.h"

// • Complete the previous program by adding a function i2c_write which will write the
// contents of the TWDR register of the microcontroller and send it to the temperature
// sensor.
// • You must write a function i2c_read which will display the contents of the TWDR
// register after the sensor measurement.
// • You must write a function print_hex_value which will write the contents of the
// 7 bytes of an AHT20 sensor measurement without modification to the standard
// output of your PC.
// • You must display the return values in a loop that repeats with a frequency that
// meets the manufacturer’s recommendations.

// Per datasheet of the AHT20 module (https://datasheet4u.com/pdf/1551700/AHT20.pdf)
// Mesurment command of the captor is:
#define ATH20_SENSOR_MEASUREMENT_COMMAND 0xAC

#define ATH20_GET_STATUS_COMMAND 0x71


void	print_hex_value(char c)
{
	uart_printhex(c);
}

// per ATH20 datasheet p.11 7.1
void	prepSensor()
{
	// 7.1 Start Sensor
	// after power-on, wait for >= 100 ms for sensor to reach idle state
	_delay_ms(100);

	// 7.2 Start Sequence
	i2c_start();

	// 7.3 Send Command
	i2c_enter_master_transmitter();
	i2c_write(ATH20_SENSOR_MEASUREMENT_COMMAND);

	// 7.4 Sensor Reading Process
	// 1.	after power-on, wait for >= 100 ms
	_delay_ms(100);

	// 		get status word by sending 0x71
	i2c_write(ATH20_GET_STATUS_COMMAND);
	uart_printstr("\r\nSensor status is :");
	uart_printhex(getI2cStatusCode());

	//		if status word is not 0x18, calibrate
	// if (status != I2C_MT_SLA_W_ACK)
	// 	ath20_calibrate();

	// 2. wait 10ms to send command
	_delay_ms(10);
	i2c_write(ATH20_SENSOR_MEASUREMENT_COMMAND);

	// 3. wait 80ms AND for read status word bit[7] to be 0
	// for measurement to be completed
	_delay_ms(80);
	while (TWDR & 0x1)
		;
}

int main()
{
	uart_init();
	i2c_init();






	while (1)
	{
		_delay_ms(20);
		char c = i2c_read();
		print_hex_value(c);
		_delay_ms(80);
	}
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
