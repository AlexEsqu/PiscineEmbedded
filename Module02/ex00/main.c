#include <avr/io.h>
#include <avr/interrupt.h>
#include <utils/delay.h>

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

// UCSRnA - USART Control and Status Register n A
// 	RXCn		TXCn		UDREn		FEn			DORn		UPEn		U2Xn		MPCMn
//	0			0			1			0			0			0		
//	[rcv empty]	[trx empty]	[ready rcv]	[frame err]	[overrun]	[parityerr]	[doubleSpd]	[multiprocessor] 

// UCSRnB - USART Control and Status Register n B
// 	RXCIEn		TXCIEn		UDRIEn		RXENn		TXENn		UCSZn2		RXB8n		TXB8n
//	0			0			1			0			0			0		
//	[int on rx]	[int on tx]	[intonURDR]	[rcv enabl]	[trx enabl]	[char size]	[9 bit rcv]	[9th bit trx] 

// UCSRnC - USART Control and Status Register n C
// 	UMSELn1		UMSELn0		UPMn1		UPMn0		USBSn		UCSZn1		UCSZn0		UCPOLn
//	0			0			1			0			0			0		
//	[set sync/async mode  ]	[ set parity mode     ]	[ stopbit ]	[set bits/frames      ]	[clock pol	]

// UBRRnH - USART Baud Rate Register High
// 	-			-			-			-			UBRRn		UBRRn		UBRRn		UBRRn
//	0			0			0			0			0			0			0			0
//	[ Reserved									  ]	[ Baud rate high bits 							]

// UBRRnL - USART Baud Rate Register Low
// 	UBRRn		UBRRn		UBRRn		UBRRn		UBRRn		UBRRn		UBRRn		UBRRn
//	0			0			0			0			0			0			0			0
//	[ Baud rate low bits 																			]



void uart_init()
{
	UBRR0H = (unsigned char)(UART_BAUDRATE >> 8);
	UBRR0L = (unsigned char) UART_BAUDRATE;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	UCSR0C = (1<<USBS0) | (3<<UCSZ00);
}

void uart_tx(char c)
{
	// Waiting for buffer to be empty
	while ( !( UCSRnA & (1 << UDREn)) )
		;

	// adding data
	UDRn = c;
}



int main()
{

}
