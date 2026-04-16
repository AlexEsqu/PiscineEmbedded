#include "libalex_avr.h"

// • You must write a program that sends an echo on the serial port, but transforms
//   lowercase characters into uppercase and uppercase characters into lowercase before
//   sending them back.
// • Attention, this time instead of using your uart_rx, you must use an interruption
//   to detect that a new character is on the UART port.
// • The infinite loop of the program must remain empty.

char transformForSubject(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		c ^= (1 << 5); // flip the 5th bit in asci to get upper/lower
	return c;
}


// specific function name which is automatically launched when USART is set
// (which is when data has been received)
// see 20.7.3 Receive Compete Flag and Interrupt & Table 12-1 of interrupts p. 66
// 
void __attribute__((signal)) __vector_18 (void)
{
	char recv = UDR0;
	uart_tx(transformForSubject(recv));
}

int main()
{
	uart_init();

	// Set up VR Status Register (SREG) to allow for interrupts
	// See Global Interrupt Enable at datasheet p. 20
	SREG |= (1 << 7);

	// Set up USART to allow for interrupts:
	// Bit 1 RXCIE0 of the USART Control and Status Register 0 B (UCSR0B) 
	// See datasheet p.201
	UCSR0B |= (1 << RXCIE0);

	while (1)
	{
		;
	}
}
