#include "libalex_avr.h"

// Read the value of RV1 with your ADC.
// • The LEDs D6 to D8 should light up according to the value of RV1 as a digital
// gauge would display it.
// ◦ LED D6: 33%
// ◦ LED D7: 66%
// ◦ LED D8: 100%

int main()
{
	spi_init();
	adc_init();

	uint32_t	colorD6, colorD7, colorD8;

	while (1)
	{
		uint32_t adc = get_adc0_conv();
		colorD6 = 0x000000;
		colorD7 = 0x000000;
		colorD8 = 0x000000;
		if (adc >= 337)
			colorD6 = 0x202020;
		if (adc >= 675)
			colorD7 = 0x202020;
		if (adc == 1023)
			colorD8 = 0x202020;

		spi_led(colorD6, colorD7, colorD8);
	}
}


