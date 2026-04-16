#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

# define BAUDRATE 115200

// • The AVR ATmega328P microcontroller has 1 UART device that you must use in
// 	 this exercise to communicate with a computer.
// • On the PC, the screen program is used to read the serial port from a terminal.
// • You must write a function uart_init that initializes the UART.
// • A function uart_tx that writes a character to the PC’s serial port.
// • The MCU’s UART must be configured as 115200 8N1.
// • UBRRnL must be calculated based on UART_BAUDRATE and F_CPU.
// • The program should write ’Z’ to the serial port at 1Hz (do as you wish).

// void uart_tx(char c);


// Initializing the USART, per 20.5 Initialization p. 184 - 185
// Using 115200 as Baudrate

// UCSR0A - USART Control and Status Register n A
// 	RXCn		TXCn		UDREn		FEn			DORn		UPEn		U2Xn		MPCMn
//	0			0			0			0			0			0			1			0
//	[rcv empty]	[trx empty]	[ready rcv]	[frame err]	[overrun]	[parityerr]	[doubleSpd]	[multiprocessor] 

// UCSR0B - USART Control and Status Register n B
// 	RXCIE0		TXCIE0		UDRIE0		RXEN0		TXEN0		UCSZ02		RXB80		TXB80
//	0			0			0			0			0			0			0			0
//	[int on rx]	[int on tx]	[intonURDR]	[rcv enabl]	[trx enabl]	[char size]	[9 bit rcv]	[9th bit trx] 

// UCSR0C - USART Control and Status Register n C
// 	UMSEL01		UMSEL00		UPM01		UPM00		USBS0		UCSZ01		UCSZ00		UCPOL0
//	0			0			1			0			0			1			1			0	
//	[set sync/async mode  ]	[ set parity mode     ]	[ stopbit ]	[set bits/frames      ]	[clock pol	]

// UBRR0H - USART Baud Rate Register High
// 	-			-			-			-			UBRRn		UBRRn		UBRRn		UBRRn
//	0			0			0			0			0			0			0			0
//	[ Reserved									  ]	[ Baud rate high bits 							]

// UBRR0L - USART Baud Rate Register Low
// 	UBRRn		UBRRn		UBRRn		UBRRn		UBRRn		UBRRn		UBRRn		UBRRn
//	0			0			0			0			0			0			0			0
//	[ Baud rate low bits 																			]


// int round()
// {

// }

// calculate UBRRn register setting based on Table 20-1 p.182
// ( F_CPU / (16 * BaudRate) ) - 1
void	setBaudRate()
{
	unsigned int ubrr = ( F_CPU / (8 * BAUDRATE) ) - 1;
	
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char) ubrr;
}


void uart_init()
{
	setBaudRate();
	
	UCSR0A = (1 << U2X0);					// set DOUBLE SPEED to lessen rounding error
	UCSR0B = (1 << TXEN0); 				// enable transmit
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);	// set stop bit and bit/frame
}

void uart_tx(unsigned char c)
{
	// Waiting for buffer to be empty
	while ( !( UCSR0A & (1 << UDRE0)) )
		;

	// adding data
	UDR0 = c;
}


int main()
{
	uart_init();

	while (1)
	{
		uart_tx('Z');
		_delay_ms(1000);
	}
}
