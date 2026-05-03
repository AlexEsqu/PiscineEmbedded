#include "libalex_avr.h"

// I am going to ask you to simply turn on a LED for this exercise.
// And since it’s D(ay)9, you’ll have to make the D9 LED blink at 1Hz.
// Unfortunately, the D9 LED is not directly connected to the MCU.
// Now that you’re all grown up, it’s up to you to find a solution!


// address of the PCA9955, per PCA9955 datasheet p.7
# define PCA_ADDRESS 0b110000

typedef enum
{
	LED_9 = 7
} e_pca_pin;

typedef enum
{
	LEDOUT0 = 0x15,
	LEDOUT1 = 0x16
} e_pca_register;


int main()
{
	i2c_init();
	uart_init();

	while (1)
	{
		 i2c_start();
		i2c_enter_master_transmitter(PCA_ADDRESS);
		uart_printstr("Address: ");
		uart_printhex(getI2cStatusCode());
		uart_printstr("\r\n");

		i2c_write(0x04);
		uart_printstr("After register write: ");
		uart_printhex(getI2cStatusCode());
		uart_printstr("\r\n");

		i2c_write(0x04);
		uart_printstr("After mode write: ");
		uart_printhex(getI2cStatusCode());
		uart_printstr("\r\n");

		i2c_write(0x13);  // PWM9 register
		uart_printstr("After PWM9 addr: ");
		uart_printhex(getI2cStatusCode());
		uart_printstr("\r\n");

		i2c_write(0xFF);  // max brightness
		uart_printstr("After brightness: ");
		uart_printhex(getI2cStatusCode());
		uart_printstr("\r\n");

		i2c_stop();
		delay_ms(500);
	}
}
