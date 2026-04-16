#include "libalex_avr.h"

// • You must write a function uart_printstr that will be called every 2 seconds to
//   display Hello World! on the serial port.
// • The MCU’s UART must be configured to 115200 8N1.
// • The infinite loop of the program should remain empty.

// void uart_printstr(const char* str);

// specific function name which is automatically launched when 
// timer1 has a compare match - see table 12-6 line 12 p74
void __attribute__((signal)) __vector_11 (void)
{
	uart_printstr("Hello World!\r\n");
}

int main()
{
	uart_init();

	// setting top of timer1 at 1 seconds
	unsigned int topOfCounter = (2UL * F_CPU / 1024UL) - 1;

	OCR1A = topOfCounter;

	timer_init(TIMER_MODE_CTC, TOP_OCR1A, CMP_SET, CMP_DISCONNECT);

	// Set up VR Status Register (SREG) to allow for interrupts
	// See Global Interrupt Enable at datasheet p. 20
	SREG |= (1 << 7);

	// Set up Timer1 to allow for interrupts:
	// Bit 1 OCIE1A of the TIMSK1 – Timer1 Interrupt Mask Register 
	// See datasheet p.145
	TIMSK1 |= (1 << OCIE1A);

	// Set up Timer prescaler which launches the timer, with prescaler 
	TCCR1B |= 0b00000101;

	while (1)
	{
		;
	}
}
