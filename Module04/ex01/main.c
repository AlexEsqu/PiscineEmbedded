#include "libalex_avr.h"

// • You must configure Timer0 to trigger a periodic interrupt that varies the duty cycle
// of the LED PB1 controlled by Timer1.
// • The frequency of Timer1 must be high enough to no longer see the LED blinking.
// • Do not hesitate to use multiple Timer registers to complete this exercise.
// • The duty cycle should vary in a loop from 0% to 100% and then from 100% to 0%
//   in 1 second.

volatile uint8_t duty = 0;

// Per schema:
// LED D1


// Setting the interrupt function of the Timer0 COMA to increase duty
// Per datasheet Table 12-6 p. 77
void __attribute__((signal, used)) __vector_14 (void)
{
	static bool isRising = true;

	if (isRising && duty < 100)
		duty++;
	else if (duty == 100)
	{
		isRising = false;
		duty--;
	}
	else if (duty == 1)
	{
		isRising = true;
		duty--;
	}
	else
		duty--;

	OCR1A = (ICR1 * duty) / 100;
}


int main()
{
	// Set LED D2 as output
	DDRB |= (1 << PB1);

	// Set Status Register (SREG) to allow for interrupts
	// See Global Interrupt Enable at datasheet p. 20
	SREG |= (1 << 7);

	// Set timer0 Status Register to allow for interrupts on COMA
	// See Timer/Counter Interrupt Mask Register p.118
	TIMSK0 |= (1 << OCIE0A);

	// Set timer1 TOP to fit one second with a 8 predivided
	ICR1 = 250;

	// Set timer1 in fast PWD with a top set by ICR1 register to pop every second
	timer1_init(TIMER_MODE_FAST_PWM, TOP_ICR, CMP_CLEAR, CMP_DISCONNECT);

	OCR0A = 77;

	// Set timer0 in CTC with a top set by OCR0A register to pop and vary the duty cycle
	timer0_init(TIMER_MODE_CTC, TOP_OCRA, CMP_TOGGLE, CMP_DISCONNECT);

	timer0_launch(CLK_DIV1024);

	timer1_launch(CLK_DIV8);

	while (1)
	{
		;
	}
}
