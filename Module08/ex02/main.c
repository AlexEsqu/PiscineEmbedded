#include "libalex_avr.h"

// Write a program that turns on the LEDs D6 to D8 successively.
// • The total period of one cycle should be 1 second.
// • The duration of each LED lighting should be 250ms.

int main()
{
	spi_init();

	while (1)
	{
		for (e_spi_led led = SPI_LED_D6; led < 3; led++)
		{
			spi_led(led, 0x010101);
			_delay_ms(250);
		}
		spi_led(NONE, 0x000000);
		_delay_ms(250);
	}
}


