#include "avr/io.h"

// • You must write a program that turns on and off the LED D2 (PB1) at a frequency
// of around 1Hz.
// • You must write code that allows you to wait for several hundred milliseconds and
// which will be inserted into the infinite loop of the program (no hardware TIMER).
// • The change in state of the LED must be made with a single bitwise operation, do
// not use a condition ( if else ).
// • You must use only the AVR registers ( DDRX, PORTX, PINX ).


// The devboard runs at 16MHz
// This busy loop in ASM is
// .L4:
// 		sbi 0x5,1
// 		std Y+2,r19
// 		std Y+1,r18
// .L2:
// 		ldd r24,Y+1
// 		ldd r25,Y+2
// 		or r24,r25
// 		breq .L4
// Per datasheet p.625:
// - sbi take 2 clocks
// - std take 2 clocks
// - ldd take 2 clocks
// - or  take 1 clock
// - bre take 1 clock
// So something like 12 clocks cycle per loop


static inline void delay_500ms(void)
{
	volatile unsigned long i = 500000UL;

	while (i)
	{
		i--;
	}
}

int main()
{
	// setting LED D2 (PB1) as output
	DDRB |= (1 << PB1);
	// initializing LED D2 (PB1) at 0 (turned off)
	// PORTB &= ~(1 << PB1);

	while (1)
	{
		PORTB ^= (1 << PB1);
		delay_500ms();
	}
}
