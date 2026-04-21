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

	// initialize the ADC
	ADMUX = 0;

	// set ADC0 as input
	// per datasheet Table 24-4 Input Channel Selection
	// set MUX at 0 0 0 0 so nothing to do

	while (1)
	{
		;
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
