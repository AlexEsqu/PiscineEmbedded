#include "avr/io.h"
#include "util/delay.h"

// • You are required to write a program that turns on and off LED D2 (PB1) at a
// frequency of 1Hz with a variable duty cycle going from 10 to 100%.
// • You must configure the registers of Timer1 to control the LED.
// • The duty cycle must be managed as follow:
//  ◦ Pressing button SW1 increments the duty cycle by 10 %.
//  ◦ Pressing button SW2 decrements the duty cycle by 10 %.

bool isBeingPressed(int buttonPort, int buttonAddress)
{
    return ((buttonPort & (buttonPort << buttonAddress)) == 0);
}


int main()
{
	// initializing LED D2 (PB1) as output
	DDRB |= 0b00000010;

    // setting up buttons to read data
    DDRD &= ~(1 << PD2) | (1 << PD4);

    // Initializing the buttons to their current states
    unsigned char sw1_was_pressed = isBeingPressed(PIND, PD2);
    unsigned char sw2_was_pressed = isBeingPressed(PIND, PD4);

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

	// Set the TOP for the Timer:
	// Toggle every 500ms (so that the LED is tuned on AND off every minute
	// = F_CPU / (howManyTogglePreFrequency * prescaler * blinkFrequency) - oneCuzIndexingAt0
	// Here 16 000 000 / (2 * 256 * 1) - 1 = 31499
	unsigned int topOfCounter = F_CPU / (2 * 256 * 1) - 1;
    unsigned int tenPercentOfTop = topOfCounter / 10;

	ICR1 = topOfCounter;

	// Set the duty cycle of the LED, meaning the count where it stops
	// Shoudl be TOP / 10 for 10%
	OCR1A = tenPercentOfTop;

	// Initialize the timer at 0 (but should be base behaviour)
	TCNT1 = 0;

	while (1)
	{
		if (!isBeingPressed(PIND, PD2) && !isBeingPressed(PIND, PD4))
            continue;

        if (!sw1_was_pressed && isBeingPressed(PIND, PD2))
        {
            if (OCR1A <= topOfCounter - tenPercentOfTop)
                OCR1A += tenPercentOfTop;
            
            sw1_was_pressed = true;
            while (isBeingPressed(PIND, PD2))
                _delay_ms(30);
        }
        if (sw1_was_pressed && !isBeingPressed(PIND, PD2))
        {
            sw1_was_pressed = false;
            while (isBeingPressed(PIND, PD2))
                _delay_ms(30);
        }

        if (!sw2_was_pressed && isBeingPressed(PIND, PD4))
        {
            if (OCR1A > tenPercentOfTop)
                OCR1A -= tenPercentOfTop;

            sw2_was_pressed = true;
            while (isBeingPressed(PIND, PD4))
                _delay_ms(30);
        }
        if (sw2_was_pressed && !isBeingPressed(PIND, PD4))
        {
            sw2_was_pressed = false;
            while (isBeingPressed(PIND, PD4))
                _delay_ms(30);
        }
	}
}
