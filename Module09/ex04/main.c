#include "libalex_avr.h"

// Now let’s focus on the 7-segment display.
// • You need to write a program that displays "2" on the rightmost digit.
// • The other 3 digits should not be lit.

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
	TOF_CA_1	= 4,
	TOF_CA_2	= 5,
	TOF_CA_3	= 6,
	TOF_CA_4	= 7
} e_pca_register0_bit_address;

typedef enum
{
	TOF_SEG_A	= 0,
	TOF_SEG_B	= 1,
	TOF_SEG_C	= 2,
	TOF_SEG_D	= 3,
	TOF_SEG_E	= 4,
	TOF_SEG_F	= 5,
	TOF_SEG_G	= 6,
	TOF_DPX		= 7
} e_pca_register1_bit_address;


typedef enum
{
	SEG_ZERO	= 0b00111111,  // A,B,C,D,E,F
	SEG_ONE		= 0b00000110,  // B,C
	SEG_TWO		= 0b01011011,  // A,B,D,E,G
	SEG_THREE	= 0b01001111,  // A,B,C,D,G
	SEG_FOUR	= 0b01100110,  // B,C,F,G
	SEG_FIVE	= 0b01101101,  // A,C,D,F,G
	SEG_SIX		= 0b01111101,  // A,C,D,E,F,G
	SEG_SEVEN	= 0b00000111,  // A,B,C
	SEG_EIGHT	= 0b01111111,  // all
	SEG_NINE	= 0b01101111   // A,B,C,D,F,G
} e_segmented_digit;

typedef enum
{
	RIGHTMOST_DIGIT	= (~(1 << TOF_CA_1)),
	RIGHTMID_DIGIT	= (~(1 << TOF_CA_2)),
	LEFTMID_DIGIT	= (~(1 << TOF_CA_3)),
	LEFTMOST_DIGIT	= (~(1 << TOF_CA_4)),
} e_segment_pos;

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

void	pca_write_FAST(e_pca_register_bytes reg, uint8_t byte1, uint8_t byte2)
{
	i2c_start();

	// ADDRESS byte (cf p.6 PCA9555 datasheet)
	// Decide to address to the PCA expander
	i2c_write(PCA_ADDRESS << 1 | 0);

	// COMMAND byte (cf p.6 PCA9555 datasheet)
	// Decide to address a specific register in the expander
	i2c_write(reg);

	// using auto swqithc to write to the next register (OUTPUT0 > OUTPUT1 for ex)
	i2c_write(byte1);

	i2c_write(byte2);
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


int main()
{
	i2c_init();
	uart_init();

	// configurating the switch (IO0_0) as input, others are output
	pca_write(CONFIGURATION_PORT_0, 0b00001111);

	// configurating all PORT_1 as output
	pca_write(CONFIGURATION_PORT_1, 0b00000000);

	i2c_start();

	// ADDRESS byte (cf p.6 PCA9555 datasheet)
	// Decide to address to the PCA expander
	i2c_enter_master_transmitter(PCA_ADDRESS);

	while (1)
	{
		pca_write_FAST(OUTPUT_PORT_0, RIGHTMOST_DIGIT, SEG_FOUR);
		delay_ms(2);

		pca_write_FAST(OUTPUT_PORT_0, RIGHTMID_DIGIT, SEG_TWO);
		delay_ms(2);

		// pca_write_FAST(OUTPUT_PORT_0, LEFTMID_DIGIT, SEG_SIX);
		delay_ms(2);

		// pca_write_FAST(OUTPUT_PORT_0, (uint8_t)LEFTMOST_DIGIT, SEG_EIGHT);
		delay_ms(2);
	}
}
