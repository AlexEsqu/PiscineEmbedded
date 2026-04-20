#include "libalex_avr.h"

volatile uint8_t g_secondsInState = 0;
volatile e_animation g_animation = NO_ANIM;
volatile uint8_t g_anim_step = 0;
volatile uint8_t g_anim_ticks = 0;
volatile bool g_anim_done = false;

void	startCountdown()
{
	// set RGB LED at orange
	set_rgb(0x40,0x06,00);

	// resetting all the COUNT LEDS to 1, leaving the rest of PORTB alone
	PORTB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);

	// start the countdown
	g_secondsInState = 0;
}

void	startRace()
{
	// set RGB LED at green
	set_rgb(0x00,0x60,00);

	g_secondsInState = 0;
}

void	startAnimation(e_animation anim)
{
	g_animation = anim;
	g_anim_step = 0;
	g_anim_ticks = 0;
	g_anim_done = false;
}
