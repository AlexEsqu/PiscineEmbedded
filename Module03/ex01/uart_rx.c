#include "libalex_avr.h"

// Waiting for the data to be received, then return the character
// Completely stolen from datasheet p. 190 Code Exmple
char uart_rx()
{
	// Waiting for data to be received
	while ( !( UCSR0A & (1 << RXC0)) )
		;

	// getting data from appropriate register
	return UDR0;
}