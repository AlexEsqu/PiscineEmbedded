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

#define DEBOUNCE_COUNT 20

// Set up External Interrupt Mask Register (EIMSK)
// to allow for interupts on SW1 press
// See register details on datasheet p.81
void	setInterruptOnSwt1()
{
	EIMSK |= (1 << INT0);
}

// Remove External Interrupt Mask Register (EIMSK)
// against interupts on SW1 press
void	removeInterruptOnSwt1()
{
	EIMSK &= ~(1 << INT0);
}

void	stopDebounceOnSwt1()
{
	// re-enable interrupt on button
	setInterruptOnSwt1();

	// remove COMA interrupt on timer
	TIMSK1 &= ~(1 << OCIE1A);
}

void	concludeDebounceOnSwt1()
{
	stopDebounceOnSwt1();

	// if button is not still pressed, was probably faulty
	if (PIND & (1 << PD2))
		return;

	// toggle LED
	PORTB ^= (1 << PB1);

	// skip while button pressed
	while (!(PIND & (1 << PD2)))
		;
}


// INTERRUPT for timer1 COMA
void	__attribute__((signal)) __vector_11 (void)
{
	concludeDebounceOnSwt1();
}

// Launch fast timer to check the button press actually was a button press
void	launchDebounceOnSwt1()
{
	// set COMA value to when debounce is over
	OCR1A = TCNT1 + DEBOUNCE_COUNT;

	// enable COMA interrupt on timer
	TIMSK1 |= (1 << OCIE1A);

	removeInterruptOnSwt1();
}


// Setting the interrupt function of the External Interrupt Request 0
// Per datasheet Table 12-6 p. 77
void __attribute__((signal)) __vector_1 (void)
{
	launchDebounceOnSwt1();
}


int main()
{
	// Set LED B1 as output and turned off
	DDRB |= (1 << PB1);
	PORTB &= ~(1 << PB1);

	// Set SWT 1 as input and initialize it (still not sure last part is needed)
	DDRD &= ~(1 << PD2);
	PORTD |= (1 << PD2);

	// Set up Status Register (SREG) to allow for interrupts (equivalent to sei())
	// See Global Interrupt Enable at datasheet p. 20
	SREG |= (1 << 7);

	setInterruptOnSwt1();

	// Set up External Interrupt Control Register A (EICRA)
	// to specify the sense control of the interrupt
	// here fall edge, so it activate as I press the button
	EICRA |= (1 << ISC11);

	// Set timer1 to be used for debouncing the switches
	timer1_init(TIMER_MODE_CTC,TOP_DEFAULT,CMP_DISCONNECT,CMP_DISCONNECT);

	timer1_launch(CLK_DIV1024);

	while (1)
	{
		;
	}
}
