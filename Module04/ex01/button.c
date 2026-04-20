#include "libalex_avr.h"

typedef enum
{
	LOW_LEVEL = 0b00,
	ANY_LOGIC = 0b01,
	FALL_EDGE = 0b10,
	RISE_EDGE = 0b11
} e_sense_control;

// Set up External Interrupt Mask Register (EIMSK)
// to allow for interupts on SW1 press
// See register details on datasheet p.81
void	setInterruptOnSwt1()
{
	EIMSK |= (1 << INT0);
}

// Remove External Interrupt Mask Register (EIMSK)
// against interupts on SW1 press
void	removeInterruptOnSwt1()
{
	EIMSK &= ~(1 << INT0);
}

// Disable button for DEBOUNCE_TICK, based on timer1B
void	setDebounceTimer()
{
	removeInterruptOnSwt1();
	g_debounce_tick = c_max_debounce_tick;
}

// ON INTERRUPT ON SW1
// Setting the interrupt function of the External Interrupt Request 0
// Per datasheet Table 12-6 p. 77
// void __attribute__((signal)) __vector_1 (void)
// {
// 	;
// }
