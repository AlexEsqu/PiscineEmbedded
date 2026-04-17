#include "libalex_avr.h"

// You must write a program that controls the RGB LED D5.
// • The LED should turn on in red, then green, then blue in a loop.
// • Changing every 1 second.



// Per Schema:
// PD3 = LED_RED
// PD5 = LED_GREEN
// PD6 = LED_BLUE

// Per my eyes and the board's back:
// PD3 = LED_BLUE
// PD5 = LED_RED
// PD6 = LED_GREEN

typedef enum
{
	LED_BLUE	= PD3,
	LED_RED		= PD5,
	LED_GREEN	= PD6
} e_color_led;




void colorOnly(e_color_led color)
{
	if (color == LED_BLUE)
	{
		clear(LED_RED, &PORTD);
		clear(LED_GREEN, &PORTD);
		set(LED_BLUE, &PORTD);
	}
	else if (color == LED_RED)
	{
		clear(LED_BLUE, &PORTD);
		clear(LED_GREEN, &PORTD);
		set(LED_RED, &PORTD);
	}
	else
	{
		clear(LED_RED, &PORTD);
		clear(LED_BLUE, &PORTD);
		set(LED_GREEN, &PORTD);
	}
}

void shiftRgb()
{
	colorOnly(LED_RED);
	_delay_ms(333);
	colorOnly(LED_GREEN);
	_delay_ms(333);
	colorOnly(LED_BLUE);
	_delay_ms(333);
}


int main()
{
	DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);

	while (1)
	{
		shiftRgb();
	}
}
