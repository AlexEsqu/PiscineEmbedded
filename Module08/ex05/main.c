#include "libalex_avr.h"

// • Write a program that will turn on the 3 LEDs D6, D7, and D8 and address the
//   following issues:
// 	◦ The potentiometer RV1 is used to change the value of one of the 3 RGB
//   	primary colors.
// 	◦ The button SW1 is used to validate the value of the potentiometer and switch
//   	to the next primary color.
// 	◦ The button SW2 is used to switch from one LED to the next.


spi_led_config_t g_ledConfig = {0};

#define DEBOUNCE_COUNT 200


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

	if (value <= 0)
		value = 15;
	else
		value--;

	displayNumber(value);

	while (!(PIND & (1 << PD4)))
		;
}

void	concludeDebounceOnSwt1()
{
	stopDebounceOnSwt1();

	if (PIND & (1 << PD2))	// if button is not still pressed, was probably faulty
		return;

	if (value >= 15)
		value = 0;
	else
		value++;

	displayNumber(value);

	while (!(PIND & (1 << PD2)))
		;

}

// Set the interrupt function of the External Interrupt Request 0
// Per datasheet Table 12-6 p. 77
void __attribute__((signal)) __vector_1 (void)
{
	launchDebounceOnSwt1();
}

// Set the interrupt function of the SWT2 press
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


int main()
{
	spi_init();
	adc_init();

	// Set buttons SW1 and SW2 to read data
	DDRD &= ~(1 << PD2);
	DDRD &= ~(1 << PD4);

	// INTERRUPTS INIT

	// Set Status Register (SREG) to allow for interrupts
	// See Global Interrupt Enable at datasheet p. 20
	SREG |= (1 << 7);

	// Set swt1 Interrupt to falling edge
	EICRA ^= (1 << ISC00);

	// Set External Interrupt Mask Register to allow for interrupts on SW1 and SW2
	// See datasheet p. 81
	setInterruptOnSwt1();

	// Set Pin Change Interrupt Control Register + Pin Change Mask Register 2
	// See datasheet p.82
	setInterruptOnSwt2();

	// DEBOUNCE INIT

	// Set timer1 to be used for debouncing the switches
	timer1_init(TIMER_MODE_CTC,TOP_DEFAULT,CMP_DISCONNECT,CMP_DISCONNECT);

	timer1_launch(CLK_DIV1024);

	while (1)
	{
		;
	}
}
