#include "libalex_avr.h"

// • Complete the previous program to display the temperature and humidity in place
// of the raw values obtained earlier.
// • The temperature and humidity values should be displayed according to the following
// specifications:
// ◦ the precision displayed on the terminal must respect the recommendations
// given in the datasheet, rounded to the nearest ten.
// (e.g.: 0.0031 -> 0.01 or 0.1 -> 0.1)
// ◦ the temperature will be in degrees Celsius (ºC), the humidity in percent (%).
// ◦ the displayed measurement must be the average of the last 3 measurements
// taken by the program. The behavior for the first two measurements should be
// consistent with this rule.

// Per datasheet of the AHT20 module (https://datasheet4u.com/pdf/1551700/AHT20.pdf)
// Mesurment command of the captor is:
#define ATH20_SENSOR_MEASUREMENT_COMMAND 0xAC
#define ATH20_SENSOR_MEASUREMENT_PARAM1 0x33
#define ATH20_SENSOR_MEASUREMENT_PARAM2 0x00

#define ATH20_GET_STATUS_COMMAND 0x71


typedef struct
{
	uint32_t	status;
	uint64_t	celsiusTimes100;
	uint64_t	humidityTimes100;
	uint32_t	CTC;
}	ath20_values_t;


void	print_hex_value(char c)
{
	uart_printhex(c);
}

uint32_t	getHumidity(uint8_t humidity1, uint8_t humidity2, uint8_t humidity3Temp1)
{
	uint32_t raw = (((uint32_t)humidity1 << 12) | (uint32_t)(humidity2 << 4) | (humidity3Temp1 >> 4));
	uint32_t HumidityTimesHundred = (uint32_t)(((uint64_t)raw * 10000ULL) / 1048576ULL);

	return (HumidityTimesHundred);
}

uint32_t	getTemp( unsigned char humidity3Temp1,  unsigned char temp2,  unsigned char  temp3)
{
	uint32_t raw =  (((uint32_t)humidity3Temp1 & 0x0F) << 16)
		| ((uint32_t) temp2 << 8) | (temp3);
	int32_t celciusTimesHundred = (int32_t)((((uint64_t)raw * 20000ULL) / 1048576ULL)) - 5000;

	return (celciusTimesHundred);
}

void	displayTempInCelsius(uint32_t celciusTimesHundred)
{
	uart_printstr("Temperature: ");
	uart_itoa((uint32_t)celciusTimesHundred / 100);
	uart_printstr(",");
	if ((celciusTimesHundred % 100) < 10)
		uart_printstr("0");
	uart_utoa((uint32_t)celciusTimesHundred % 100);
	uart_printstr("°C");
}

void	displayHumidity(uint32_t HumidityTimesHundred)
{
	uart_printstr("Humidity: ");
	uart_itoa(HumidityTimesHundred / 100);
	uart_printstr(",");
	if ((HumidityTimesHundred % 100) < 10)
		uart_printstr("0");
	uart_utoa(HumidityTimesHundred % 100);
	uart_printstr("%RH");
}

// per ATH0 datasheet, p.11 7.4 Sensor Reading Process
ath20_values_t	getATH20SensorData()
{

	// SENDING COMMAND

	i2c_start();

	// 2.	Wait 10ms
	_delay_ms(10);

	//		Send command with parameters
	i2c_enter_master_transmitter();
	i2c_write(ATH20_SENSOR_MEASUREMENT_COMMAND);
	i2c_write(ATH20_SENSOR_MEASUREMENT_PARAM1);
	i2c_write(ATH20_SENSOR_MEASUREMENT_PARAM2);

	i2c_stop();

	// 3. Wait 80ms
	_delay_ms(80);


	// GETTING  DATA

	ath20_values_t	result;

	// 4. receive 7 bytes
	i2c_start();
	i2c_enter_master_receiver();
	result.status   = i2c_read();
	uint8_t	humidity1 = i2c_read();
	uint8_t	humidity2 = i2c_read();
	uint8_t	humidity3Temp1 = i2c_read();
	uint8_t	temp2 = i2c_read();
	uint8_t	temp3 = i2c_read();
	result.CTC = i2c_read();

	// 5. send NACK if no need for CRC check
	i2c_stop();

	result.humidityTimes100 = getHumidity(humidity1, humidity2, humidity3Temp1);
	result.celsiusTimes100 = getTemp(humidity3Temp1, temp2, temp3);

	return (result);
}

int main()
{
	ath20_values_t reading1 = (ath20_values_t){0};
	ath20_values_t reading2 = (ath20_values_t){0};
	ath20_values_t reading3 = (ath20_values_t){0};

	uart_init();
	i2c_init();

	// 1. Wait for Sensor to be idle
	_delay_ms(100);

	reading1 = getATH20SensorData();

	while (1)
	{
		if (reading3.status == 0x18)
		{
			displayTempInCelsius((reading1.celsiusTimes100 + reading2.celsiusTimes100 + reading3.celsiusTimes100)/3ULL);
			uart_printstr(" ");
			displayHumidity((reading1.humidityTimes100 + reading2.humidityTimes100 + reading3.humidityTimes100)/3ULL);
			uart_printstr("\r\n");
			reading1 = reading2;
			reading2 = reading3;
			reading3 = getATH20SensorData();
		}
		else if (reading2.status == 0x18)
		{
			displayTempInCelsius((reading1.celsiusTimes100 + reading2.celsiusTimes100)/2ULL);
			uart_printstr(" ");
			displayHumidity((reading1.humidityTimes100 + reading2.humidityTimes100)/2ULL);
			uart_printstr("\r\n");
			reading3 = getATH20SensorData();
		}
		else
		{
			displayTempInCelsius(reading1.celsiusTimes100);
			uart_printstr(" ");
			displayHumidity(reading1.humidityTimes100);
			uart_printstr("\r\n");
			reading2 = getATH20SensorData();
		}
		_delay_ms(500);
	}
}

