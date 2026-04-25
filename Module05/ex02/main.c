#include "libalex_avr.h"

// • Lisez le potentiomètre RV1 + la LDR (R14) + la NTC (R20).
// • Affichez ensuite les valeurs au format hexadécimal toutes les 20 ms sur la console.

// per schema:
// RV1 / Potentiometer is on ADC_POT, which is on ADC_0 and PC0

void adc_init()
{
	// Initialize ADC ports

	ADMUX = 0;
	ADCSRA = 0;
	ADCSRB = 0;

	// set PC0 as input
	DDRC &= ~(1 << PC0);

	// set voltage reference to AVCC as subject requires
	// remove ADLR since no need for 8bits only anymore
	ADMUX |= (1 << REFS0);

	// Initialize ADC Control

	// launch ADC with prescaler select of 128
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

	// launch ADC in Free Running mode
	// no set up needed
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


int main()
{
	uart_init();

	// ADC INITIALIZATION
	adc_init();

	while (1)
	{
		// print potentiometer
		uart_itoa(get_adc0_conv());
		uart_printstr(", ");

		// print LDR
		uart_itoa(get_adc1_conv());
		uart_printstr(", ");

		// print NTC
		uart_itoa(get_adc2_conv());
		uart_printstr("\r\n");
		_delay_ms(20);
	}
}


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
