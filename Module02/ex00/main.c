#include "libalex_avr.h"

// • The AVR ATmega328P microcontroller has 1 UART device that you must use in
// 	 this exercise to communicate with a computer.
// • On the PC, the screen program is used to read the serial port from a terminal.
// • You must write a function uart_init that initializes the UART.
// • A function uart_tx that writes a character to the PC’s serial port.
// • The MCU’s UART must be configured as 115200 8N1.
// • UBRRnL must be calculated based on UART_BAUDRATE and F_CPU.
// • The program should write ’Z’ to the serial port at 1Hz (do as you wish).

// void uart_tx(char c);

int main()
{
	uart_init();

	while (1)
	{
		uart_tx('Z');
		_delay_ms(1000);
	}
}
