#include "libalex_avr.h"

const uint8_t c_max_debounce_tick = 20;

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
// Setting the interrupt function of the External Interrupt Request 0 to light LED
// Per datasheet Table 12-6 p. 77
void __attribute__((signal)) __vector_1 (void)
{
	switch (g_state)
	{
		case START:
		{
			g_state = FIRST_READY; // I pressed my switch and wait for the peer to do the same
			break;
		}
		case COUNTDOWN:
		{
			g_state = LOSER;
			if (!g_is_master)
				slave_reply_byte = SLAVE_PRESSED_RACE;
			break;
		}
		case RACE:
		{
			g_state = WINNER;
			if (!g_is_master)
				slave_reply_byte = SLAVE_PRESSED_RACE;
			break;
		}
		default:
		{
			break;
		}
	}
	setDebounceTimer();
}
