#include "libalex_avr.h"

// Ok, now even more colors! You must write a program that controls the RGB LED
// D5 but with PWM. Make the colors transition from one to the next smoothly using the
// provided wheel function.
// • Write a function init_rgb() that initializes the timers.
// • Then write a function set_rgb() that allows you to set the color of the LED.
// • Finally, your program must display the color wheel.


void wheel(uint8_t pos) 
{
	pos = 255 - pos;
	if (pos < 85) 
	{
		set_rgb(255 - pos * 3, 0, pos * 3);
	} 
	else if (pos < 170) 
	{
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	} 
	else 
	{
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}


int main()
{
	
	uint8_t pos = 0;
	init_rgb();

	while (1)
	{
		wheel(pos++);
		_delay_ms(10);
	}
}
