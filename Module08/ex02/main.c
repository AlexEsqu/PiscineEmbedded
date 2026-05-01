#include "libalex_avr.h"

// Write a program that turns on the LEDs D6 to D8 successively.
// • The total period of one cycle should be 1 second.
// • The duration of each LED lighting should be 250ms.

int main()
{
	spi_init();

	while (1)
	{
		// loops over enum containing D6, D7, D8 and NONE=> 4 x 250ms one second cycles
		for (e_spi_led led = SPI_LED_D6; led < 4; led++)
		{
			spi_led(led, 0x010101);
			_delay_ms(250);
		}
	}
}


