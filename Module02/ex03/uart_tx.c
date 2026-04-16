#include "libalex_avr.h"

// Waiting for the buffer to be empty, then sending one character at a time
// Completely stolen from datasheet p. 186 Code Exmple
void uart_tx(char c)
{
	// Waiting for buffer to be empty
	while ( !( UCSR0A & (1 << UDRE0)) )
		;

	// adding data
	UDR0 = c;
}