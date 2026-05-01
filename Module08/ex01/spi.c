#include "libalex_avr.h"

// see datasheet p.207-208
void	spi_init()
{
	// Set MOSI and SCK output, all others input
	DDRB = (1<<PB2) | (1<<PB3) | (1 << PB5);

	// Set SPI as Master
	PORTB |= (1 << PB2);

	// Enable SPI, Master, set clock rate fck/16
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	// SPDR |=(1<<SPI2X);
}

void	spi_write(uint8_t c)
{
	SPDR = c;

	while(!(SPSR & (1<<SPIF)))
		;
}

void	colorD6(uint32_t rgb)
{
	// Start frame
	for (int i = 0; i < 4; i++)
		spi_write(0x00);

	// LED frame: 0xFF (brightness), then BGR
	spi_write(0xFF);				// Max brightness
	spi_write(get_blue(rgb));				// Blue = 0
	spi_write(get_green(rgb));				// Green = 0
	spi_write(get_red(rgb));				// Red = 255

	// End frame
	spi_write(0xFF);
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
