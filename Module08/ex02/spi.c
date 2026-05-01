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
	spi_write(0xFF);						// Max brightness
	spi_write(get_blue(rgb));				// Blue = 0
	spi_write(get_green(rgb));				// Green = 0
	spi_write(get_red(rgb));				// Red = 255

	// End frame
	spi_write(0xFF);
}

void	colorD7(uint32_t rgb)
{
	// Start frame
	for (int i = 0; i < 4; i++)
		spi_write(0x00);

	// D6 frame
    spi_write(0xE0);
    spi_write(0x00);
    spi_write(0x00);
    spi_write(0x00);

	// LED frame: 0xFF (brightness), then BGR
	spi_write(0xE8);						// Max brightness
	spi_write(get_blue(rgb));				// Blue = 0
	spi_write(get_green(rgb));				// Green = 0
	spi_write(get_red(rgb));				// Red = 255

	// End frame
	spi_write(0xFF);
}

void	spi_send_turn_off_frame()
{
	spi_write(0xE0);
	spi_write(0x00);
	spi_write(0x00);
	spi_write(0x00);
}

void	spi_send_color_frame(uint32_t rgb)
{
	spi_write(0xE1);
	spi_write(get_blue(rgb));
	spi_write(get_green(rgb));
	spi_write(get_red(rgb));
}

void	spi_led(e_spi_led led, uint32_t rgb)
{
	// Start frame
	for (int i = 0; i < 4; i++)
		spi_write(0x00);

	if (led == SPI_LED_D6)
		spi_send_color_frame(rgb);
	else
		spi_send_turn_off_frame(SPI_LED_D6);

	if (led == SPI_LED_D7)
		spi_send_color_frame(rgb);
	else
		spi_send_turn_off_frame(SPI_LED_D7);

	if (led == SPI_LED_D8)
		spi_send_color_frame(rgb);
	else
		spi_send_turn_off_frame(SPI_LED_D8);

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
