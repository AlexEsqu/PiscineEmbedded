#include "libalex_avr.h"

// calculate UBRRn register setting based on Table 20-1 p.182
// ( F_CPU / (16 * BaudRate) ) - 1
void	setBaudRate()
{
	unsigned int ubrr = ( F_CPU / (8 * BAUDRATE) ) - 1;
	
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char) ubrr;
}

// Initializing the USART, per 20.5 Initialization p. 184 - 185
// Mainly copying suggested code, then double speed to ease with rouding error
void uart_init()
{
	setBaudRate();
	
	UCSR0A = (1 << U2X0);					// set DOUBLE SPEED to lessen rounding error
	UCSR0B = (1 << TXEN0) | (1 << RXEN0); 					// enable receiving
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);	// set stop bit and bit/frame
}


// View of the USART registers:

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