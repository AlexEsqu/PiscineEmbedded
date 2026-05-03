#include "libalex_avr.h"

// Great! You managed to use U4 in output mode. Now let’s also test the inputs.
// You need to write a program that:
// • Every time you press the SW3 button, increment a counter.
// • And constantly display its value on the D9 D10 D11 LEDs in binary.

# define PCA_ADDRESS 0b0100000

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

typedef enum
{
	VAL_SWITCH_3	= 0b0000'0001,
	VAL_LED_D11		= 0b0000'0010,
	VAL_LED_D10		= 0b0000'0100,
	VAL_LED_D9		= 0b0000'1000,
	VAL_CA_1		= 0b0001'0000,
	VAL_CA_2		= 0b0010'0000,
	VAL_CA_3		= 0b0100'0000,
	VAL_CA_4		= 0b1000'0000
} e_pca_register0_bits;

typedef enum
{
	SWITCH_3	= 0,
	LED_D11		= 1,
	LED_D10		= 2,
	LED_D9		= 3,
	CA_1		= 4,
	CA_2		= 5,
	CA_3		= 6,
	CA_4		= 7
} e_pca_register0_bit_address;

void	pca_write(e_pca_register_bytes reg, uint8_t byte)
{
	i2c_start();

	// ADDRESS byte (cf p.6 PCA9555 datasheet)
	// Decide to address to the PCA expander
	i2c_enter_master_transmitter(PCA_ADDRESS);

	// COMMAND byte (cf p.6 PCA9555 datasheet)
	// Decide to address a specific register in the expander
	i2c_write(reg);

	i2c_write(byte);

	i2c_stop();
}

uint8_t	pca_read(e_pca_register_bytes reg)
{
	i2c_start();

	// ADDRESS byte (cf p.6 PCA9555 datasheet)
	// Decide to address to the PCA expander
	i2c_enter_master_transmitter(PCA_ADDRESS);

	// COMMAND byte (cf p.6 PCA9555 datasheet)
	// Decide to address a specific register in the expander
	i2c_write(reg);

	i2c_start();

	// ADDRESS byte (cf p.6 PCA9555 datasheet)
	// Decide to address to the PCA expander
	i2c_enter_master_receiver(PCA_ADDRESS);

	uint8_t result = i2c_read_nack();

	// uart_printstr("After read: ");
	// uart_printhex(getI2cStatusCode());
	// uart_printstr("\r\n");

	i2c_stop();

	// uart_printstr("Result: ");
	// uart_printhex(result);
	// uart_printstr("\r\n");

	return result;
}

void displayNumber(uint8_t num)
{
	// explicitly extracting all binary digit of the count
	// for those like me not really up to spec in binary
	unsigned char binary1 =  (num & 0b00000001);
	unsigned char binary2 =  ((num & 0b00000010) >> 1);
	unsigned char binary4 =  ((num & 0b00000100) >> 2);

	// setting the LEDS to their respective values
	pca_write(OUTPUT_PORT_0, (0b11111111 & ~(binary4 << LED_D11) & ~(binary2 << LED_D10) & ~(binary1 << LED_D9)));
}

int main()
{
	i2c_init();
	uart_init();

	// configurating the switch (IO0_0) as input, others are output
	pca_write(CONFIGURATION_PORT_0, 0b00000001);

	uint8_t	counter = 0;

	while (1)
	{
		uint8_t inputPort0 = pca_read(INPUT_PORT_0);

		if (!(inputPort0 & 1))
		{
			// uart_printstr("Button pressed!\r\n");
			counter++;
			if (counter == 16)
				counter = 0;

			displayNumber(counter);

			while (!(inputPort0 & 1))
			{
				inputPort0 = (pca_read(INPUT_PORT_0) & 0b00000001);
				delay_ms(50);
			}
		}



		delay_ms(50);
	}
}
