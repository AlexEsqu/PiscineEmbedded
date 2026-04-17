#include "libalex_avr.h"

// Ok now, more colors! You will have to write a program that controls the RGB LED
// D5. Here is a table of colors that should be displayed successively, in a loop and changing
// color every second.

typedef enum
{
	LED_BLUE	= PD3,
	LED_RED		= PD5,
	LED_GREEN	= PD6
} e_color_led;

typedef enum
{
	RED			= 0b0010'0000,
	GREEN		= 0b0100'0000,
	BLUE		= 0b0000'1000,
	YELLOW		= 0b0110'0000,
	CYAN		= 0b0100'1000,
	MAGENTA		= 0b0010'1000,
	WHITE		= 0b0110'1000
}	e_color;

static const e_color colorVect[] = {RED,GREEN,BLUE,YELLOW,CYAN,MAGENTA,WHITE};

void setColorLedTo(e_color color)
{
	PORTD = (PORTD & 0b1001'0111) | color;
}

void shiftRgb()
{
	for (unsigned int i = 0; i != sizeof(colorVect) / sizeof(e_color); i++)
	{
		setColorLedTo(colorVect[i]);
		_delay_ms(1000);
	}
}


int main()
{
	DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);

	while (1)
	{
		shiftRgb();
	}
}
