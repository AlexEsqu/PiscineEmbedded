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
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); 	// enable transmit and reception
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);	// set bit/frame
}


// Overview of the USART register and their use:

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
//	0			0			0			0			0			1			1			0
//	[set sync/async mode  ]	[ set parity mode     ]	[ stopbit ]	[set bits/frames      ]	[clock pol	]

// UBRR0H - USART Baud Rate Register High
// 	-			-			-			-			UBRRn		UBRRn		UBRRn		UBRRn
//	0			0			0			0			0			0			0			0
//	[ Reserved									  ]	[ Baud rate high bits 							]

// UBRR0L - USART Baud Rate Register Low
// 	UBRRn		UBRRn		UBRRn		UBRRn		UBRRn		UBRRn		UBRRn		UBRRn
//	0			0			0			0			0			0			0			0
//	[ Baud rate low bits 																			]


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

void uart_tx(char c)
{
	// Waiting for buffer to be empty
	while ( !( UCSR0A & (1 << UDRE0)) )
		;

	// adding data
	UDR0 = c;
}

void uart_printstr(const char* str)
{
    while (*str)
    {
        uart_tx(*str++);
    }
}


void uart_printhex(uint32_t num)
{
    const char hex[] = "0123456789ABCDEF";
    char buf[8];
    uint32_t i = 0;

	uart_printstr("0x");
    if (num == 0)
    {
        uart_tx('0');
        return;
    }

    while (num > 0 && i < sizeof(buf))
    {
        buf[i++] = hex[num % 16];
        num /= 16;
    }

    while (i > 0)
    {
        uart_tx(buf[--i]);
    }
}

void uart_itoa(uint32_t num)
{
    const char dec[] = "0123456789";
    char buf[8];
    uint32_t i = 0;

    if (num == 0)
    {
        uart_tx('0');
        return;
    }

    while (num > 0 && i < sizeof(buf))
    {
        buf[i++] = dec[num % 10];
        num /= 10;
    }

    while (i > 0)
    {
        uart_tx(buf[--i]);
    }
}
