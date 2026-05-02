#include "libalex_avr.h"

// • Write a program that will turn on the 3 LEDs D6, D7, and D8 and address the
// 	following issues:
// 		◦ The potentiometer RV1 is used to change the value of one of the 3 RGB
// 			primary colors.
// 		◦ The button SW1 is used to validate the value of the potentiometer and switch
// 			to the next primary color.
// 		◦ The button SW2 is used to switch from one LED to the next


// if globals are allowed, might as well...
spi_led_config_t	g_ledConfig = {0};

// for easier adc updates, instead of many many if else
uint8_t*		c_colorMap[3][3] = {
	{&g_ledConfig.ledD6.red, &g_ledConfig.ledD6.green, &g_ledConfig.ledD6.blue},
	{&g_ledConfig.ledD7.red, &g_ledConfig.ledD7.green, &g_ledConfig.ledD7.blue},
	{&g_ledConfig.ledD8.red, &g_ledConfig.ledD8.green, &g_ledConfig.ledD8.blue}
};

void	OnSw1Press()
{
	if (g_ledConfig.currentPrimary == PRIMARY_BLUE)
		g_ledConfig.currentPrimary = PRIMARY_RED;
	else
		g_ledConfig.currentPrimary++;
}


void	OnSw2Press()
{
	if (g_ledConfig.currentLed == SPI_LED_D8)
		g_ledConfig.currentLed = SPI_LED_D6;
	else
		g_ledConfig.currentLed++;
	g_ledConfig.currentPrimary = PRIMARY_RED;
}


void	updateLedBasedOnAdcValue()
{
	// using c_colorMap to look up which value to update
	// gathering 10 bits from adc, dropping last two bits since color can only be 8bits
	*c_colorMap[g_ledConfig.currentLed][g_ledConfig.currentPrimary] = (get_adc0_conv() >> 2);
}


void	spi_send_led_frame(spi_color_t* ledConfig)
{
	spi_write(0xE1);
	spi_write(ledConfig->blue);
	spi_write(ledConfig->green);
	spi_write(ledConfig->red);
}

void	spi_send_config_frames()
{
	for (int i = 0; i < 4; i++)
		spi_write(0x00);

	spi_send_led_frame(&g_ledConfig.ledD6);

	spi_send_led_frame(&g_ledConfig.ledD7);

	spi_send_led_frame(&g_ledConfig.ledD8);

	spi_write(0xFF);
}

int main()
{
	spi_init();
	button_init();
	adc_init();

	// Enable interrupt
	SREG |= (1 << 7);

	// Set timer1 for debouncing the switches
	timer1_init(TIMER_MODE_CTC,TOP_DEFAULT,CMP_DISCONNECT,CMP_DISCONNECT);
	timer1_launch(CLK_DIV1024);

	g_ledConfig.currentLed = SPI_LED_D6;
	g_ledConfig.currentPrimary = PRIMARY_RED;

	while (1)
	{
		updateLedBasedOnAdcValue();
		spi_send_config_frames();
	}
}
