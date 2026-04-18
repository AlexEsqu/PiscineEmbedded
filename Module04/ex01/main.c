#include "libalex_avr.h"

// • You must configure Timer0 to trigger a periodic interrupt that varies the duty cycle
// of the LED PB1 controlled by Timer1.
// • The frequency of Timer1 must be high enough to no longer see the LED blinking.
// • Do not hesitate to use multiple Timer registers to complete this exercise.
// • The duty cycle should vary in a loop from 0% to 100% and then from 100% to 0%
//   in 1 second.


// Per schema:
// SW1 is on PD2 / INT0
// LED D1

// Per datasheet:
// Pin Change Interrup Request 0 is vector 3 - Table 12-6 p.74
// Interrupts on switch press (external interrupts) can be set up
// Using SREG and External Interrupt Control Register A (EICRA)

typedef enum
{
	LOW_LEVEL = 0b00,
	ANY_LOGIC = 0b01,
	FALL_EDGE = 0b10,
	RISE_EDGE = 0b11
} e_sense_control;


// Setting the interrupt function of the External Interrupt Request 0 to light LED
// Per datasheet Table 12-6 p. 77
void __attribute__((signal)) __vector_1 (void)
{
	PORTB ^= (1 << PD1);
}


int main()
{
	// Set LED D1 as output
	DDRB |= (1 << PD1);
	// Set SWT 1 as input
	DDRB &= ~(1 << PD2);

	// Set up Status Register (SREG) to allow for interrupts
	// See Global Interrupt Enable at datasheet p. 20
	SREG |= (1 << 7);

	// Set up External Interrupt Mask Register (EIMSK)
	// to allow for interupts on SW1 press
	// See register details on datasheet p.81
	EIMSK |= (1 << INT0);

	// Set up External Interrupt Control Register A (EICRA)
	// to specify the sense control of the interrupt
	EICRA |= (0 << ISC01) | (1 << ISC00);

	while (1)
	{
		;
	}
}
