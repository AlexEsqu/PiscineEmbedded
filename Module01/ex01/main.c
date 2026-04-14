#include "avr/io.h"
#include "util/delay.h"

// • You must write a program that turns on and off the LED D2 (PB1)
//   at a frequency of 1Hz.
// • You must configure the registers of the Timer1 to control the LED.
// • The infinite loop of the program must remain empty.
// • And you must not use PORTX.

int main()
{
	// initializing LED D2 (PB1) as output
	DDRB |= 0b00000010;

	// Setting up Timer1 in CTC Mode and Toggle OC1A submode
		// Because it will toggle OC1A when reaching the counter TOP
		// And the LED2 I want to toggle is *coincidentally* on OC1A
		// source: datasheet p.131 - 132

	// According to Compare Output Mode non-PWD Table 16-1, p. 140:
	// For Toggle: COM1A1/COM1B1 is at 0, COM1A0/COM1B0 is at 1

	// According to Waveform Generation Mode Table 16-4 line 4 p. 141
	// WGM11, WGM10 and WGM13 are at 0, WGM12 is at 1
	// BTW why are WGM11 and WGM10 on a different byte ????

	// ICNC (Input Caputre Noise Canceler) and ICES (Input Capture Edge Select)
	// detailed in p.142 are not relevant to the exercise and thus set to 0

	// Reserved bits marked ----- are to be left at 0

	// According to Clock Select Bit Description Table 16-5 p. 143
	// Prescaling can be set at 256  with CS12  at 1 and CS11 and CS10 at 0
		// This mean Timer1 will be slowed to 16 000 000 Hz / 256 = 62 500 Hz
		// So my counter increments every 0.000016 seconds
		// Meaning I can get 0.5 seconds by waiting 31250 clocks

	// other source: https://passionelectronique.fr/introduction-timer-arduino/

	// Set Timer1
	// TCCR1A :
	// COM1A1	COM1A0	COM1B1	COM1B0	-----	-----	WGM11	WGM10
	//		0		1		0		1		0		0		0		0
	// TCCR1B :
	// ICNC1	ICES1	-----	WGM13	WGM12	CS12	CS11	CS10
	//		0		0		0		0		1		1		0		0
	TCCR1A = 0b01010000;
	TCCR1B = 0b00001100;

	// Set the TOP for the Timer:
	// Toggle every 500ms (so that the LED is tuned on AND off every minute
	// = F_CPU / (howManyTogglePreFrequency * prescaler * blinkFrequency) - oneCuzIndexingAt0
	// Here 16 000 000 / (2 * 256 * 1) - 1 = 31499
	OCR1A = F_CPU / (2 * 256 * 1) - 1;

	// Initialize the timer at 0 (but should be base behaviour)
	TCNT1 = 0;

	while (1)
	{
		;
	}
}
