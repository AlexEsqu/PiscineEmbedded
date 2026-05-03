#include "libalex_avr.h"

// • You need to retrieve the value of RV1 on 10 bits.
// • And display it on the 7-segment display

# define PCA_ADDRESS 0b0100000

uint16_t			g_counter = 0;

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
	// SEG_NONE	= 0b00000000,
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

void	displayOnSegment(e_segmented_digit rightmostDigit, e_segmented_digit rightmidDigit,
		e_segmented_digit leftmidDigit, e_segmented_digit leftmostDigit)
{
	pca_write_FAST(OUTPUT_PORT_0, RIGHTMOST_DIGIT, rightmostDigit);
	delay_ms(2);

	pca_write_FAST(OUTPUT_PORT_0, RIGHTMID_DIGIT, rightmidDigit);
	delay_ms(2);

	pca_write_FAST(OUTPUT_PORT_0, LEFTMID_DIGIT, leftmidDigit);
	delay_ms(2);

	pca_write_FAST(OUTPUT_PORT_0, (uint8_t)LEFTMOST_DIGIT, leftmostDigit);
	delay_ms(2);
}

void	displayNumOnSegment(uint16_t num, e_segmented_digit* array)
{
	e_segmented_digit buff[4];
	uint8_t	digitPos = 0;

	while (num > 0 && digitPos < 4)
	{
		buff[digitPos++] = array[num % 10];
		num /= 10;
	}

	while (digitPos < 4)
	{
		buff[digitPos++] = SEG_ZERO;
	}

	displayOnSegment(buff[3], buff[2], buff[1], buff[0]);
}

int main()
{
	i2c_init();
	adc_init();

	// configurating the switch (IO0_0) as input, others are output
	pca_write(CONFIGURATION_PORT_0, 0b00001111);

	// configurating all PORT_1 as output
	pca_write(CONFIGURATION_PORT_1, 0b00000000);

	i2c_start();

	// ADDRESS byte (cf p.6 PCA9555 datasheet)
	// Decide to address to the PCA expander
	i2c_enter_master_transmitter(PCA_ADDRESS);

	e_segmented_digit	array[] = {SEG_ZERO, SEG_ONE, SEG_TWO, SEG_THREE, SEG_FOUR, SEG_FIVE, SEG_SIX, SEG_SEVEN, SEG_EIGHT, SEG_NINE};

	while (1)
	{
		uint16_t	adc = get_adc0_conv();
		displayNumOnSegment(adc, array);
	}
}
