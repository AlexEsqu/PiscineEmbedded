#include "libalex_avr.h"
#include <stdbool.h>

uint8_t g_is_master = 0;
volatile bool g_peer_ready = false;
volatile bool g_peer_pressed = false;
volatile e_state g_state = START;

// PC5 : SDA / ADC4, data of the Two Wire
// PC4 : DCL / ADC5, clock of the Two Wire

void	displayErrorLED(void)
{
	set_rgb(255, 80, 0);
}

void gpio_init(void)
{
    // LEDs D1..D4
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);

    // LED RGB
    DDRD |= (1 << LED_BLUE) | (1 << LED_RED) | (1 << LED_GREEN);

    // Bouton SW1 avec pull-up
    DDRD  &= ~(1 << PD2);
    PORTD |=  (1 << PD2);
}

// =============================================================================
// MAIN
// =============================================================================
int main(void)
{
    // --- INIT HARDWARE ---
    uart_init();
    twi_init();
    gpio_init();

    // Timer0/2 pour PWM RGB
    setTimer0Timer2ForRGB();

    fight_for_control();

    _delay_ms(10);

    // --- ACTIVATION DES MÉCANISMES ---
    // À partir d'ici, les interrupts Timer1A, Timer1B, INT0 sont actives
    setGeneralTimer();

    // INT0 sur front descendant (bouton)
    EICRA = (EICRA & ~(1 << ISC00)) | (1 << ISC01);
    setInterruptOnSwt1();

    // Active global interrupts
    SREG |= (1 << 7);

    g_state = START;
    static e_state s_prevState = ERROR;

    // --- BOUCLE PRINCIPALE ---
    while (1)
    {
        // === ENTRY ACTIONS (quand l'état change) ===
        handle_state_entry(&s_prevState);

        // === POLLING I2C ===
        twi_polling();

        // === RESET APRÈS ANIMATION ===
        if ((g_state == WINNER || g_state == LOSER) && g_anim_done)
		{
            PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4));
            if (!g_is_master)
                slave_reply_byte = SLAVE_IDLE;
            g_state = START;
            g_anim_done = false;
        }
    }

    return 0;
}
