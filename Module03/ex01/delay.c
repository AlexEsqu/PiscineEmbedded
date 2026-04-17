#include "libalex_avr.h"

void delay_ms(int ms)
{
	volatile unsigned long i = (F_CPU / 2) / 35 / 500;
    i *= ms;

	while (i)
	{
		i--;
	}
}