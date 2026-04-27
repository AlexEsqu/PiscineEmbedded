#include "libalex_avr.h"

// Write a program that initializes SPI as master and turns on the LED D6 in red.

// void	setSPIBaudRate()
// {
// 	unsigned int ubrr = ( F_CPU / (2 * BAUDRATE) ) - 1;

// 	UBRR0H = (unsigned char)(ubrr >> 8);
// 	UBRR0L = (unsigned char) ubrr;
// }


// see datasheet p.207-208
void	spi_init()
{
	/* Set MOSI and SCK output, all others input */
	DDRB = (1<<PB3) | (1<<PB4) | (1 << PB5);

	PORTB |= (1 << PB2);

	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR);
	SPDR |=(1<<SPI2X);
}

void	spi_write(uint8_t c)
{
	SPDR = c;

	while(!(SPSR & (1<<SPIF)))
		;
}


int main()
{
	spi_init();

	while (1)
	{
		// Start frame
		for (int i = 0; i < 4; i++)
			spi_write(0x00);

		// LED frame: 0xFF (brightness), then BGR
		spi_write(0xFF);				// Max brightness
		spi_write(0x00);				// Blue = 0
		spi_write(0x00);				// Green = 0
		spi_write(0xFF);				// Red = 255

		// End frame
		for (int i = 0; i < 4; i++)
			spi_write(0xFF);
	}
}

// OVERVIEW OF THE REGISTERS
//
// SPCR – SPI Control Register
// 	SPIE		SPE			DORD		MSTR		CPOL		CPHA		SPR1		SPR0
//	[intEnable]	[SPIenable]	[LSB/MSB]	[master/sl]	[polarity]	[clkphase]	[ clock rate select		]

// SPSR – SPI Status Register
// 	SPIF		WCOL		–			–			–			–			–			SPI2X
//	[SPIinterr]	[WriteColl]	[														]	[doubleSpeed]

// SPDR – SPI Data Register
// 	MSB																					LSB
//
