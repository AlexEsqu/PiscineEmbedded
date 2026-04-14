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

	// Setting up Timer1 in Phase Correct PWD Mode and inverse Compare Output submode
		// Because it will clear OC1A when incrementing to a duty value I set in ICR1
		// Keep incrementing, then set when reaching this value decrementing
		// no need for frequency correct mode since not changing the TOP value
		// source: datasheet p.134 - 132

	// According to Compare Output Mode Phase Correct PWD Table 16-3, p. 141:
	// For Reverse Compare Output Submode: COM1A1/COM1B1 is at 1, COM1A0/COM1B0 is at 0

	// According to Waveform Generation Mode Table 16-4 line 10 p. 141:
	// TCCR1A :
	// COM1A1	COM1A0	COM1B1	COM1B0	-----	-----	WGM11	WGM10
	//		1		0		1		0		0		0		1		0
	// TCCR1B :
	// ICNC1	ICES1	-----	WGM13	WGM12	CS12	CS11	CS10
	//		0		0		0		1		0		1		0		0
	TCCR1A = 0b10100010;
	TCCR1B = 0b00010100;

	// Set the TOP for the Timer:
	// Toggle every 500ms (so that the LED is tuned on AND off every minute
	// = F_CPU / (howManyTogglePreFrequency * prescaler * blinkFrequency) - oneCuzIndexingAt0
	// Here 16 000 000 / (2 * 256 * 1) - 1 = 31499
	int topOfCounter = F_CPU / (2 * 256 * 1) - 1;

	ICR1 = topOfCounter;

	// Set the duty cycle of the LED, meaning the count where it stops
	// Shoudl be TOP / 10 for 10%
	OCR1A = topOfCounter / 10;

	// Initialize the timer at 0 (but should be base behaviour)
	TCNT1 = 0;

	while (1)
	{
		;
	}
}

