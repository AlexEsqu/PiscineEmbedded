#include "libalex_avr.h"

// I am going to ask you to simply turn on a LED for this exercise.
// And since it’s D(ay)9, you’ll have to make the D9 LED blink at 1Hz.
// Unfortunately, the D9 LED is not directly connected to the MCU.
// Now that you’re all grown up, it’s up to you to find a solution!


// Ressource : PCA9555 datasheet
// https://www.nxp.com/docs/en/data-sheet/PCA9555.pdf

// see PCA datasheet p.6
# define PCA_ADDRESS 0b0100000

// see PCA datasheet p.6
typedef enum
{
	INPUT_PORT_0,
	INPUT_PORT_1,
	OUTPUT_PORT_0,
	OUTPUT_PORT_1,
	POLARITY_PORT_0,
	POLARITY_PORT_1,
	CONFIGURATION_PORT_0,
	CONFIGURATION_PORT_1
} e_pca_register_bytes;


int main()
{
	i2c_init();
	// uart_init();

	i2c_start();

	// ADDRESS byte (cf p.6 PCA9555 datasheet)
	// shifted cuz the function will shift it again
	// Decide to address to the PCA expander
	i2c_enter_master_transmitter(PCA_ADDRESS);

	// uart_printhex(getI2cStatusCode());

	// COMMAND byte (cf p.6 PCA9555 datasheet)
	// Decide to address a specific register in the expander
	// Here the configuration port to set LED D9 as output
	i2c_write(CONFIGURATION_PORT_0);

	// uart_printhex(getI2cStatusCode());

	// CONFIGURE Led D9 / pin 7 as
	i2c_write(0b00000000);

	// uart_printhex(getI2cStatusCode());

	i2c_stop();

	while (1)
	{
		i2c_start();

		// ADDRESS byte (cf p.6 PCA9555 datasheet)
		// shifted cuz the function will shift it again
		// Decide to address to the PCA expander
		i2c_enter_master_transmitter(PCA_ADDRESS);

		// uart_printhex(getI2cStatusCode());

		// COMMAND byte (cf p.6 PCA9555 datasheet)
		// Decide to address a specific register in the expander
		// Here the output port to transmit the led ligthing command
		i2c_write(OUTPUT_PORT_0);

		// uart_printhex(getI2cStatusCode());

		// opposite polarity: 0 lights pup, 1 turns off
		i2c_write(0b11110111);

		// uart_printhex(getI2cStatusCode());

		i2c_stop();

		delay_ms(1000);
	}
}
