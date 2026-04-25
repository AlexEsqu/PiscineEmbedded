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
#define ATH20_SENSOR_MEASUREMENT_PARAM1 0x33
#define ATH20_SENSOR_MEASUREMENT_PARAM2 0x00

#define ATH20_GET_STATUS_COMMAND 0x71


void	print_hex_value(char c)
{
	uart_printhex(c);
}

void	printHumidity(unsigned char humidity1, unsigned char humidity2, unsigned char humidity3Temp1)
{
	uint32_t raw = ((humidity1 << 12) | (humidity2 << 4) | (humidity3Temp1 >> 4));
	uint32_t integerHumidity = (raw * 10000UL) / (float)1048576;

	uart_printstr("Humidity: ");
	uart_itoa(integerHumidity / 100);
	uart_printstr(",");
	uart_itoa(integerHumidity);
	uart_printstr("%RH from ");
	uart_itoa(raw);
}

void	printTemp( unsigned char humidity3Temp1,  unsigned char temp2,  unsigned char  temp3)
{
	(void) humidity3Temp1;
	(void) temp2;
	(void) temp3;
	uint32_t raw =  ((uint32_t)(humidity3Temp1 & 0x0F) << 16) | ((uint32_t) temp2 << 8) | (temp3);
	float celcius = ((raw * 200 / (float)1048576) - 50);

	uart_printstr(" Temperature: ");
	uart_itoa(celcius);
	uart_printstr("C from ");
	uart_utoa(raw);

	// 4294960781
	// 1048576

}

// per ATH0 datasheet, p.11 7.4 Sensor Reading Process
void	getATH20SensorData()
{
	i2c_start();

	// 2.	Wait 10ms
	_delay_ms(10);

	//		Send command with parameters
	i2c_enter_master_transmitter();
	i2c_write(ATH20_SENSOR_MEASUREMENT_COMMAND);
	i2c_write(ATH20_SENSOR_MEASUREMENT_PARAM1);
	i2c_write(ATH20_SENSOR_MEASUREMENT_PARAM2);

	// 3. Wait 80ms
	_delay_ms(80);

	//		AND for read status word bit[7] to be 0
	i2c_renew_start();
	i2c_enter_master_receiver();
	// char status = i2c_read();
	// while (status & 0x80)
	// 	status = i2c_read();

	// 4. receive 6 bytes
	char	humidity1 = i2c_read();
	char	humidity2 = i2c_read();
	char	humidity3Temp1 = i2c_read();
	char	temp2 = i2c_read();

	// 5. send NACK if no need for CRC check
	char	temp3 = i2c_read();
	i2c_read_and_stop();

	printHumidity(humidity1, humidity2, humidity3Temp1);
	printTemp(humidity3Temp1, temp2, temp3);
	uart_printstr("\r\n");

	_delay_ms(500);
}

int main()
{
	uart_init();
	i2c_init();

	// 1. Wait for Sensor to be idle
	_delay_ms(100);

	while (1)
	{
		getATH20SensorData();
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
