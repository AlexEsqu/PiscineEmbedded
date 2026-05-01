#include "libalex_avr.h"

// Do you remember exercise 1 from Module04? Let’s do it again!
// • You will have to write a program that controls the RGB LED D6.
// • Here is a table of colors that must be displayed successively.
// name R# G# B# color
// red ff 0 0
// green 0 ff 0
// blue 0 0 ff
// yellow ff ff 0
// cyan 0 ff ff
// magenta ff 0 ff
// white ff ff ff
// • In a loop and changing color every second.


int main()
{
	spi_init();

	const uint32_t colors[] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0x00FFFF, 0xFF00FF, 0xFFFFFF};
	const uint8_t len = sizeof(colors) / sizeof(uint32_t);

	while (1)
	{
		for (int i = 0; i < len; i++)
		{
			colorD6(colors[i]);
			_delay_ms(1000);
		}
	}
}


