#include "libalex_avr.h"


// • Lisez le potentiomètre RV1 + la LDR (R14) + la NTC (R20).
// • Affichez ensuite les valeurs au format hexadécimal toutes les 20 ms sur la console.

// per schema:
// RV1 / Potentiometer is on ADC_POT, which is on ADC_0 and PC0

// Hackerman function to get the board's fuse and lock bits
// #include <avr/boot.h>
	// avr/boot.h getsystembyte reference offset register 0x0002 to get voltage offset
// int main()
// {
// 	uart_init();
// 	uint32_t offset = boot_lock_fuse_bits_get(GET_LOCK_BITS);
// 	uart_itoa(offset);
// }


uint16_t	getkCoefficient(uint16_t adcA, uint16_t adcB, uint16_t TempA, uint16_t TempB)
{
	return (adcA - adcB) / (TempA - TempB);
}


uint16_t	getTOS(uint16_t adcA, uint16_t k, uint16_t tempA)
{
	return (adcA - (k * tempA));
}

// Calibration functions to guess at the values in 24.8 Temperature Measurement
// See datasheet p.256-257
void	getCalibration()
{
	uart_printstr("Please air out the chip:");
	uint16_t adcConvA = get_adc8_conv();

	uart_printstr("\r\nPlease warm up the chip:");
	_delay_ms(2000);
	uint16_t adcConvB = get_adc8_conv();
	uint16_t asssumedTempInF1BTodayInCelsiusAccordingToVelmir = 23;
	uint16_t TempAsIWarmUpTheBoardInMyKnittedHat = 24;

	uint16_t k = getkCoefficient(adcConvA, adcConvB, asssumedTempInF1BTodayInCelsiusAccordingToVelmir, TempAsIWarmUpTheBoardInMyKnittedHat);
	uint16_t TOS = getTOS(adcConvA, k, asssumedTempInF1BTodayInCelsiusAccordingToVelmir);

	uart_init();

	uart_printstr("Likely k:");
	uart_itoa(k);
	uart_printstr("\r\nLikely TOS:");
	uart_itoa(TOS);
}


int main()
{
	uart_init();
	adc_init();

	// getCalibration();

	while (1)
	{
		uart_itoa(get_adc8_conv());
		uart_printstr("\r\n");
		_delay_ms(20);
	}
}

