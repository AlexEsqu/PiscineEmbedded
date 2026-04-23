#include "libalex_avr.h"


// • Lisez le potentiomètre RV1 + la LDR (R14) + la NTC (R20).
// • Affichez ensuite les valeurs au format hexadécimal toutes les 20 ms sur la console.

#define TOP_OF_TEN_BITS 1023

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
	adc_init();
	init_rgb();
	uart_init();
	DDRB |= (1 << LED_D1) | (1 << LED_D2) | (1 << LED_D3) | (1 << LED_D4);

	while (1)
	{
		uint16_t rv1 = get_adc0_conv();
		uint32_t percentage = ((float)rv1 / (float)TOP_OF_TEN_BITS) * 255;
		wheel(percentage);
		PORTB &= ~((1 << LED_D1) | (1 << LED_D2) | (1 << LED_D3) | (1 << LED_D4));
		if (percentage >= 64)
		{
			PORTB |= (1 << LED_D1);
		}
		if (percentage >= 128)
		{
			PORTB |= (1 << LED_D2);
		}
		if (percentage >= 192)
		{
			PORTB |= (1 << LED_D3);
		}
		if (percentage >= 255)
		{
			PORTB |= (1 << LED_D4);
		}
	}
}

