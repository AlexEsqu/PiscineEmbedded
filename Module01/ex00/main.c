#include "avr/io.h"

// • You must write a program that turns on and off the LED D2 (PB1) at a frequency
// of around 1Hz.
// • You must write code that allows you to wait for several hundred milliseconds and
// which will be inserted into the infinite loop of the program (no hardware TIMER).
// • The change in state of the LED must be made with a single bitwise operation, do
// not use a condition ( if else ).
// • You must use only the AVR registers ( DDRX, PORTX, PINX ).


// When converted to ASM (see make asm rule), the loop amounts to:
// .L3:
// 	ldd r24,Y+1
// 	ldd r25,Y+2
// 	ldd r26,Y+3
// 	ldd r27,Y+4
// 	sbiw r24,1
// 	sbc r26,__zero_reg__
// 	sbc r27,__zero_reg__
// 	std Y+1,r24
// 	std Y+2,r25
// 	std Y+3,r26
// 	std Y+4,r27
// .L2:
// 	ldd r24,Y+1
// 	ldd r25,Y+2
// 	ldd r26,Y+3
// 	ldd r27,Y+4
// 	sbiw r24,0
// 	cpc r26,__zero_reg__
// 	cpc r27,__zero_reg__
// 	brne .L3

// Using the Instruction Set Summary p.625 of the datasheet,
// This is around 35 cycles (not accounting for loop overhead cuz this is ex00)

static inline void delay_500ms(void)
{
	// How many time should I wast 35 clock cycles to reach 500ms ?
	// => secondsPerClockCycle * cyclePerLoop * numberOfLoops  = 0.5 seconds
	// => numberOfLoops = ((1 / F_CPU) * 35) / 0.5
	// => numberOfLoops = ( 0.5 * F_CPU ) / 35
	volatile unsigned long i = (F_CPU / 2) / 35;

	while (i)
	{
		i--;
	}
}

int main()
{
	// setting LED D2 (PB1) as output
	DDRB |= (1 << PB1);
	// initializing LED D2 (PB1) at 0 (turned off), made by default
	// PORTB &= ~(1 << PB1);

	while (1)
	{
		PORTB ^= (1 << PB1);
		delay_500ms();
	}
}
