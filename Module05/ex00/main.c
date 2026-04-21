#include "libalex_avr.h"

// Lisez la valeur du potentiomètre linéaire RV1 en utilisant le peripherique ADC.
// • ADC doit être configuré avec une résolution de 8 bits et AVCC comme référence.
// • Affichez ensuite sa valeur au format hexadécimal toutes les 20 ms sur la console.

// Per schema:
// SW1 is on PD2 / INT0
// LED D1

// per schema:
// RV1 / Potentiometer is on ADC_POT, which is on ADC_0 and PC0

int main()
{
	uart_init();

	// set PC0 as input
	DDRC &= ~(1 << PC0);

	// ADC INITIALIZATION

	// Initialize ADC ports
	ADMUX = 0;
	// set ADC0 as input is not needed
	// per datasheet Table 24-4 Input Channel Selection
	// set MUX at 0 0 0 0 to target ADC so nothing to do
	// set voltage reference to AVCC as subject requires
	ADMUX |= (1 << REFS0) | (1 << ADLAR);

	// Initialize ADC Control
	ADCSRA = 0;
	// launch ADC with prescaler select of 128
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

	// launch ADC in Free Running mode
	ADCSRB = 0;

	while (1)
	{
		// require conversion
		ADCSRA |= (1 << ADSC);
		// wait for conversion to be over (p. 258)
		while (ADCSRA & (1 << ADSC))
			;

		uint8_t high = ADCH;
		uart_printstr("Potentialometer is:");
		uart_printhex(high);
		uart_printstr("\r\n");
		_delay_ms(500);
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
