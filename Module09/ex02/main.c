#include "libalex_avr.h"

// Now let’s focus on the 7-segment display.
// • You need to write a program that displays "2" on the rightmost digit.
// • The other 3 digits should not be lit.


// Ressource : LED SEGMENT DATASHEET
// https://jlc-prod-smt.oss-eu-central-1.aliyuncs.com/smtDataManualFile/8588946567709798400-C325426.pdf?response-content-disposition=attachment%3B%20filename%3DC325426.pdf%3B%20filename%2A%3DUTF-8%27%27C325426.pdf&x-oss-date=20260503T125416Z&x-oss-expires=1800&x-oss-security-token=CAISgAN1q6Ft5B2yfSjIr5rsKOmApelnwpiJVH6AgHglNOBEvKLguzz2IHhMdHJsAOodtv0%2FmmhT6PkclqRLcbhpcmfjV%2BZHzLB8qcdTphN34J7b16cNrbH4M4H6aXeirtuwDsz9SNTCALjPD3nPii50x5bjaDymRCbLGJaViJlhHLN1Ow6jdmhpCctxLAlvo9NgFxm3D%2Fu2NQPwiWf9FVdhvhEG6Vly8qOi2MaRmFy8yFTx0b0SvJ%2BjYMrmPctoN9JnSdC5mfdzau3a1TJ84gRD0a5wkaVA1zbDs5bfISEIuUzebreLqY03dV4mOvdqIcMe8qigz88fk%2FfIioH6xyxKOexoSCnFTOiiupCcQLPyao9jLu6iayqViY7QaIOTqQohZmkAMwVOasAsI3Ngh4zF97Qt0cVNkXO9gWfLI8DtuMleWqruR9Zb7nSgc5lCkRRYwGs1287ugXlSQzo890KPDAEovaKCnZ2ZSfh7Y4sNknI6i%2Bfc2Se2MIkIGuVbMKKWD5sagAF%2FNUlhupkevdpz7wsP85W2zneTIQfwrt1OsNwApRT%2B50IWENMYnjjlhpwDveDU8zMPteqm5IoW%2Bm%2BQMIqDS84PBH1ke%2BsNJMEivJ4jVBr90wuKb%2FGBMQn122e5PhJQwBPonE1DWnRAP8TBiMsK1o9I4GKuPC15N7Qsji8Do3BfQCAA&x-oss-signature-version=OSS4-HMAC-SHA256&x-oss-credential=STS.NYYcS4H6FuZbVX1dxp8ohSgKY%2F20260503%2Feu-central-1%2Foss%2Faliyun_v4_request&x-oss-signature=80fa65dd86c809ea23e84425a30bdaff1bee66c9c1c7a8b765ac0e4b5a90e39d

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
	RIGHTMOST_DIGIT	= 0b00111111,
	RIGHTMID_DIGIT	= 0b00000110,
	LEFTMID_DIGIT	= 0b01011011,
	LEFTMOST_DIGIT	= 0b01001111,
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
	pca_write(OUTPUT_PORT_0, (~(binary4 << LED_D11) & ~(binary2 << LED_D10) & ~(binary1 << LED_D9)));
}

int main()
{
	i2c_init();
	uart_init();

	// configurating the switch (IO0_0) as input, others are output
	pca_write(CONFIGURATION_PORT_0, 0b00000001);

	// configurating all PORT_1 as output
	pca_write(CONFIGURATION_PORT_1, 0b00000000);

	while (1)
	{
		pca_write(OUTPUT_PORT_0, (~(1 << TOF_CA_1)));
		pca_write(OUTPUT_PORT_1, SEG_TWO);
		delay_ms(50);
	}
}
