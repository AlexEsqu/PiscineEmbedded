#include "libalex_avr.h"


// ADMUX – ADC Multiplexer Selection Register
// REFS1		REFS0		ADLAR		–			MUX3		MUX2		MUX1		MUX0
// [ Voltage reference	  ]	[Left Adju]	[		]	[ which analog inputs are connected		]

// ADCSRA – ADC Control and Status Register A
// ADEN			ADSC		ADATE		ADIF		ADIE		ADPS2		ADPS1		ADPS0
// [enable ADC]	[StartConv]	[Auto Trig]	[Int Flag]	[Int Enabl]	[Prescaler select bits		]

// ADCSRB – ADC Control and Status Register B
// –			ACME		–			–			–			ADTS2		ADTS1		ADTS0
// 				[multiplex]										[							]

// ACSR - Analog Comparator and Status Register
// ACD			ACBG		ACO			ACI			ACIE		ACIC		ACIS1		ACIS0
// [comp off]	[bandgap]	[output]	[int flag]	[int enabl]	[input cap]	[interrupt mode	]


// ADCL - ADC Data Register Low
// ADCH - ADC Data Register High
// Two Registers contain result of conversion

// ADCSRB – ADC Control and Status Register B
// –			ACME		–			–			–			ADTS2		ADTS1		ADTS0
// [		  ]	[multiplex] [								  ]	[Auto Trigger Source		]

void adc_init()
{
	// Initialize ADC ports

	ADMUX = 0;
	ADCSRA = 0;
	ADCSRB = 0;

	// set PC0 as input
	DDRC &= ~(1 << PC0);

	// set voltage reference to AVCC
	ADMUX |= (1 << REFS0);

	// Initialize ADC Control

	// launch ADC with prescaler select of 128
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

	// launch ADC in Free Running mode
	// no set up needed
}

uint32_t getAdcConv(void)
{
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC))
		;
	return ADC;
}


uint16_t get_adc0_conv()
{
	// set ADC to check input ADC0 / Potentiometer
	// see Table 24-4 Input Channel Selection
	ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));

	// require conversion
	ADCSRA |= (1 << ADSC);
	// wait for conversion to be over (p. 258)
	while (ADCSRA & (1 << ADSC))
		;

	// extracting ten first bits of data
	uint16_t tenBitsOfData = (ADC & 0b0000001111111111);

	return tenBitsOfData;
}

uint16_t get_adc1_conv()
{
	// set ADC to check input ADC0 / Potentiometer
	// see Table 24-4 Input Channel Selection
	ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1));
	ADMUX |= (1 << MUX0);

	// require conversion
	ADCSRA |= (1 << ADSC);
	// wait for conversion to be over (p. 258)
	while (ADCSRA & (1 << ADSC))
		;

	uint16_t tenBitsOfData = (ADC & 0b0000001111111111);

	return tenBitsOfData;
}

uint16_t get_adc2_conv()
{
	// set ADC to check input ADC0 / Potentiometer
	// see Table 24-4 Input Channel Selection
	ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX0));
	ADMUX |= (1 << MUX1);

	// require conversion
	ADCSRA |= (1 << ADSC);
	// wait for conversion to be over (p. 258)
	while (ADCSRA & (1 << ADSC))
		;

	uint16_t tenBitsOfData = (ADC & 0b0000001111111111);

	return tenBitsOfData;
}


// additional source: https://developerhelp.microchip.com/xwiki/bin/view/products/mcu-mpu/8-bit-avr/peripherals/internal-temp-sensor/adc/
uint16_t get_adc8_conv()
{
	ADMUX = (1<<REFS1) | (1<<REFS0)							// Vref = internal 1.1V
		| (0<<ADLAR)										// Right aligned
		| (1<<MUX3) | (0<<MUX2) | (0<<MUX1) | (0<<MUX0);	// ADC8

    /* Perform Dummy Conversion to complete ADC init */
    ADCSRA |= (1<<ADSC);

    /* wait for conversion to complete */
    while ((ADCSRA & (1<<ADSC)) != 0)
		;

	/* start a new conversion on channel 8 */
	ADCSRA |= (1<<ADSC);

	/* wait for conversion to complete */
	while ((ADCSRA & (1<<ADSC)) != 0)
	;

	/* Calculate the temperature in C */
	uint32_t Ctemp = (ADC - 30);


	// Restore base ADC setup
	ADMUX = (1 << REFS0);

	return (Ctemp);
}



