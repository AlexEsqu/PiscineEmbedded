#include "avr/io.h"

// • You are required to write a program that turns on and off LED D2 (PB1) at a
// frequency of 1Hz with a duty cycle of 10%.
// • You must configure the registers of Timer1 to control the LED.
// • The infinite loop of your program must remain empty.
// • You must not use PORTX


int main()
{
	// initializing LED D2 (PB1) as output
	DDRB |= 0b00000010;

	// Setting up Timer1 in Fast PWD Mode and non inverse Compare Output submode
		// Because it will clear OC1A when incrementing to a duty value I set in ICR1
		// Keep incrementing, then set when reaching TOP and looping back to BOTTOM value
		// source: datasheet p.132 - 134

	// According to Compare Output Mode Fast PWD Table 16-3, p. 141:
	// For Reverse Compare Output Submode: COM1A1/COM1B1 is at 1, COM1A0/COM1B0 is at 0

	// According to Waveform Generation Mode Table 16-4 line 14 p. 142:
	// TCCR1A :
	// COM1A1	COM1A0	COM1B1	COM1B0	-----	-----	WGM11	WGM10
	//		1		0		1		0		0		0		1		0
	// TCCR1B :
	// ICNC1	ICES1	-----	WGM13	WGM12	CS12	CS11	CS10
	//		0		0		0		1		1		1		0		0
	TCCR1A = 0b10100010;
	TCCR1B = 0b00010100;

	// Set Timer TOP at 1second
	// Fast PWM period is (TOP + 1), so (F_CPU / prescaler / target_hz) - indexingAt0.
	unsigned int topOfCounter = (F_CPU / 256UL) - 1;

	// Set the duty cycle of the LED where clears the LED
	unsigned int tenPercentOfTop = topOfCounter / 10;

	ICR1 = topOfCounter;

	OCR1A = tenPercentOfTop;

	// Initialize the timer at 0 (but should be base behaviour)
	TCNT1 = 0;

	while (1)
	{
		;
	}
}

