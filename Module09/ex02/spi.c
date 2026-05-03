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

void	spi_send_turn_off_frame()
{
	spi_write(0xE0);
	spi_write(0x00);
	spi_write(0x00);
	spi_write(0x00);
}

void	spi_send_color_frame(uint32_t rgb)
{
	if (rgb == 0x000000)
	{
		spi_send_turn_off_frame();
	}
	else
	{
		spi_write(0xE1);
		spi_write(get_blue(rgb));
		spi_write(get_green(rgb));
		spi_write(get_red(rgb));
	}
}

void	spi_send_one_led_frames(e_spi_led led, uint32_t rgb)
{
	// Start frame
	for (int i = 0; i < 4; i++)
		spi_write(0x00);

	if (led == SPI_LED_D6)
		spi_send_color_frame(rgb);
	else
		spi_send_turn_off_frame();

	if (led == SPI_LED_D7)
		spi_send_color_frame(rgb);
	else
		spi_send_turn_off_frame();

	if (led == SPI_LED_D8)
		spi_send_color_frame(rgb);
	else
		spi_send_turn_off_frame();

	// End frame
	spi_write(0xFF);
}

void	spi_send_all_led_frames(uint32_t rgbLedD6, uint32_t rgbLedD7, uint32_t rgbLedD8)
{
	// Start frame
	for (int i = 0; i < 4; i++)
		spi_write(0x00);

	spi_send_color_frame(rgbLedD6);
	spi_send_color_frame(rgbLedD7);
	spi_send_color_frame(rgbLedD8);

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
