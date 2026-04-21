#include "libalex_avr.h"

// SWITCH 1 - External interrupt

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

// SWITCH 2 - Alternate Function of D Port
// See Table 14-9 p.97
// Per Schema, switch 2 is on port D 4
// Which has as alternate PCINT20

// Set Pin Change Interrupt Control Register (PCICR)
// to allow for interrupts on PCMSK2 pins (PCINT 16 to 23)
// Set Pin Change Mask Register 2 (PCMSK2)
// to allow for interupts on PCINT20 set (happens to be SW2)
// See register details on datasheet p.81
void	setInterruptOnSwt2()
{
	PCICR |= (1 << PCIE2); // allow for interrupt
	PCMSK2 |= (1 << PCINT20);
}

// Remove Pin Change Mask Register 2 (PCMSK2)
// against interupts on PCINT20 set (happens to be SW2)
void	removeInterruptOnSwt2()
{
	PCMSK2 &= ~(1 << PCINT20);
}

