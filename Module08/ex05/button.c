#include "libalex_avr.h"


// SWITCH 1 - External interrupt

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

// SWITCH 2 - Alternate Function of D Port
// See Table 14-9 p.97
// Per Schema, switch 2 is on port D 4
// Which has as alternate PCINT20

// Set Pin Change Interrupt Control Register (PCICR)
// to allow for interrupts on PCMSK2 pins (PCINT 16 to 23)
// Set Pin Change Mask Register 2 (PCMSK2)
// to allow for interupts on PCINT20 set (happens to be SW2)
// See register details on datasheet p.81
void	setInterruptOnSwt2()
{
	PCICR |= (1 << PCIE2); // allow for interrupt
	PCMSK2 |= (1 << PCINT20);
}

// Remove Pin Change Mask Register 2 (PCMSK2)
// against interupts on PCINT20 set (happens to be SW2)
void	removeInterruptOnSwt2()
{
	PCMSK2 &= ~(1 << PCINT20);
}

// Launch fast timer to check the button press actually was a button press
void	launchDebounceOnSwt1()
{
	// set COMA value to when debounce is over
	OCR1A = TCNT1 + DEBOUNCE_COUNT;

	// enable COMA interrupt on timer
	TIMSK1 |= (1 << OCIE1A);

	removeInterruptOnSwt1();
	EICRA ^= (1 << ISC00);
}

// Launch fast timer to check the button press actually was a button press
void	launchDebounceOnSwt2()
{
	// set COMB value to when debounce is over
	OCR1B = TCNT1 + DEBOUNCE_COUNT;

	// enable COMA interrupt on timer
	TIMSK1 |= (1 << OCIE1B);

	removeInterruptOnSwt2();

}

void	stopDebounceOnSwt1()
{
	setInterruptOnSwt1(); // re-enable interrupt on button

	// remove COMA interrupt on timer
	TIMSK1 &= ~(1 << OCIE1A);
}

void	stopDebounceOnSwt2()
{
	setInterruptOnSwt2(); // re-enable interrupt on button

	// remove COMB interrupt on timer
	TIMSK1 &= ~(1 << OCIE1B);
}

void	concludeDebounceOnSwt2()
{
	stopDebounceOnSwt2();

	if (PIND & (1 << PD4))	// if button is not still pressed, was probably faulty
		return;

	OnSw2Press();

	while (!(PIND & (1 << PD4)))
		;
}

void	concludeDebounceOnSwt1()
{
	stopDebounceOnSwt1();

	if (PIND & (1 << PD2))	// if button is not still pressed, was probably faulty
		return;

	OnSw1Press();

	while (!(PIND & (1 << PD2)))
		;

}

void	button_init()
{
	// Set buttons SW1 and SW2 to read data
	DDRD &= ~(1 << PD2);
	DDRD &= ~(1 << PD4);
	PORTD |= (1 << PD2) | (1 << PD4);

	// Set swt1 Interrupt to falling edge
	EICRA ^= (1 << ISC00);

	// Set External Interrupt Mask Register to allow for interrupts on SW1 and SW2
	// See datasheet p. 81
	setInterruptOnSwt1();

	// Set Pin Change Interrupt Control Register + Pin Change Mask Register 2
	// See datasheet p.82
	setInterruptOnSwt2();
}

// Set the interrupt function of the External Interrupt Request 0
// Per datasheet Table 12-6 p. 77
void __attribute__((signal)) __vector_1 (void)
{
	launchDebounceOnSwt1();
}

// Set the interrupt function of the External Interrupt Request 0
// Per datasheet Table 12-6 p. 77
void __attribute__((signal)) __vector_5 (void)
{
	launchDebounceOnSwt2();
}

// Set the interrupt function of the Timer1 COMA
// Per datasheet Table 12-6 p. 77
void __attribute__((signal)) __vector_11 (void)
{
	concludeDebounceOnSwt1();
}


// Set the interrupt function of the Timer1 COMB
// Per datasheet Table 12-6 p. 77
void __attribute__((signal)) __vector_12 (void)
{
	concludeDebounceOnSwt2();
}
