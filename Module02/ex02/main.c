#include "libalex_avr.h"

// • Now you will have to write a function uart_rx that waits to receive a character on
// the MCU’s UART port and then returns it.
// • You must write a program that uses your uart_rx function.
// • It should write the received characters from uart_rx to the serial port using your
// uart_tx function (ex00).

// char uart_rx(void);


int main()
{
	uart_init();
	
	while (1)
	{
		char recv = uart_rx();
		uart_tx(recv);
	}
}

