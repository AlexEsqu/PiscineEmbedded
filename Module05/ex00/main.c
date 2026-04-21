#include "libalex_avr.h"

// Lisez la valeur du potentiomètre linéaire RV1 en utilisant le peripherique ADC.
// • ADC doit être configuré avec une résolution de 8 bits et AVCC comme référence.
// • Affichez ensuite sa valeur au format hexadécimal toutes les 20 ms sur la console.

// Per schema:
// SW1 is on PD2 / INT0
// LED D1

// Per datasheet:
// Pin Change Interrup Request 0 is vector 3 - Table 12-6 p.74
// Interrupts on switch press (external interrupts) can be set up
// Using SREG and External Interrupt Control Register A (EICRA)

int main()
{
	uart_init();

	while (1)
	{
		;
	}
}
