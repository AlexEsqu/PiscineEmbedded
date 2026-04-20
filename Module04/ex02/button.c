#include "libalex_avr.h"


void	setInterruptLogic(e_switch button, e_sense_control logic)
{
	if (e_switch = BUTTON_SW1)
		EICRA = (EICRA & 0b1111'1100) | logic;
	else
		EICRA = (EICRA & 0b1111'0011) | logic << 2;
}

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
