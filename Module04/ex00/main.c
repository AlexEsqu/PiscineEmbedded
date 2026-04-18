#include "libalex_avr.h"

// • You are required to write a program that changes the state of LED D1 when the
// button SW1 is pressed.
// • You must use interrupts to read the state of the button. Reading the PINx registers
// is not allowed.


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


void	__attribute__((signal)) __vector_11 (void)
{
	// re-enable the interrupt
	EIMSK |= (1 << INT0);

	// stop the timer
	timer0_launch(CLK_STOP);
}

void	setupDebounceTimer()
{
	// set debounce time at 1 second
	OCR1A = ((F_CPU / 256UL) - 1) /2;

	// initilize timer1 at 0
	TCNT1 = 0;

	// intialize a timer1 to track debounce time
	timer1_init(TIMER_MODE_CTC, TOP_OCRA, CMP_SET, CMP_DISCONNECT);

	// launch timer1 by setting up redivider of 256
	timer1_launch(CLK_DIV256);
}


// Setting the interrupt function of the External Interrupt Request 0 to light LED
// Per datasheet Table 12-6 p. 77
void __attribute__((signal)) __vector_1 (void)
{
	// toggle LED
	PORTB ^= (1 << PD1);

	// disable switch external interrupt
	EIMSK &= ~(1 << INT0);

	// launch timer which will interrupt later to reenable
	setupDebounceTimer();
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
	EICRA |= (1 << ISC01) | (0 << ISC00);

	while (1)
	{
		;
	}
}
