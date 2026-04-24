#include "libalex_avr.h"


// • Lisez le potentiomètre RV1 + la LDR (R14) + la NTC (R20).
// • Affichez ensuite les valeurs au format hexadécimal toutes les 20 ms sur la console.

// per schema:
// RV1 / Potentiometer is on ADC_POT, which is on ADC_0 and PC0

// Hackerman function to get the board's fuse and lock bits
#include <avr/boot.h>

// avr/boot.h getsystembyte reference offset register 0x0002 to get voltage offset
// int main()
// {
// 	uart_init();
// 	uint32_t offset = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
// 	uart_itoa(offset);
// }


int main()
{
	uart_init();
	adc_init();

	while (1)
	{
		uart_itoa(get_adc8_conv());
		uart_printstr("\r\n");
		_delay_ms(20);
	}
}

