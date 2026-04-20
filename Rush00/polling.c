#include "libalex_avr.h"

// =============================================================================
// HELPERS I2C
// =============================================================================

// Poll I2C côté slave : vérifie ce qu'on reçoit du master
// Met à jour g_state directement si besoin
static void slave_poll_and_act(void)
{
    uint8_t data = 0;
    uint8_t st = twi_slave_poll(&data);
    if (st != 0x80)
		return;   // pas de data reçue

    switch (data)
    {
		case MSG_HELLO:
			// Le master a rebooté - on a déjà ACKé son HELLO donc il nous
			// considère comme slave. On reste slave et on reset notre jeu.
			if (g_state != START) {
				PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4));
				g_animation = NO_ANIM;
				g_anim_done = false;
				g_state = START;
				slave_reply_byte = SLAVE_IDLE;
			}
			break;
		case MSG_GO_COUNTDOWN:
            if (g_state == FIRST_READY || g_state == START)
            {
                startCountdown();
                g_state = COUNTDOWN;
            }
            break;
        case MSG_FALSE_START_ME:
            // Le master a faux-départé → on a gagné
            if (g_state == COUNTDOWN) {
				g_state = WINNER;
			}
            break;
        case MSG_FALSE_START_YOU:
            // Le master dit qu'on a faux-départé
            if (g_state == COUNTDOWN) {
           		g_state = LOSER; 
			}
            break;
        case MSG_YOU_WIN:
            if (g_state == RACE || g_state == COUNTDOWN) {
				g_state = WINNER; 
			}
            break;
        case MSG_YOU_LOSE:
            if (g_state == RACE || g_state == COUNTDOWN) {
            	g_state = LOSER; 
			}
            break;
    }
}


void twi_polling(void)
{
	if (g_is_master)
	{
		// Master lit l'état du slave périodiquement
		static uint8_t skip = 0;
		static uint8_t peer_zero_count = 0;
		if ((skip++ & 0x0F) == 0)
		{   // pas trop souvent
			uint8_t s = twi_master_read_slave();
 
			// Détection slave reseté : reply_byte = 0x00 (valeur par défaut
			// d'une MCU fraîchement bootée, avant que la négociation ne
			// positionne SLAVE_IDLE). On exige 2 lectures consécutives.
			if (s == 0x00 && g_state != START && g_state != RENEGOTIATE)
			{
				if (++peer_zero_count >= 2)
				{
					peer_zero_count = 0;
					g_state = RENEGOTIATE;
					return;
				}
			}
			else
			{
				peer_zero_count = 0;
			}

			// Cas 1: on attend le peer (FIRST_READY) et il est prêt
			if (g_state == FIRST_READY && s == SLAVE_READY)
			{
				twi_master_send_msg(MSG_GO_COUNTDOWN);
				_delay_us(200);
				startCountdown();
				g_state = COUNTDOWN;
			}
			// Cas 2: pendant COUNTDOWN, slave a faux-départé
			else if (g_state == COUNTDOWN && s == SLAVE_FALSE_START)
			{
				twi_master_send_msg(MSG_FALSE_START_YOU);
				g_state = WINNER;
			}
			// Cas 3: pendant RACE, slave a appuyé
			else if (g_state == RACE && s == SLAVE_PRESSED_RACE)
			{
				twi_master_send_msg(MSG_YOU_WIN);
				g_state = LOSER;
			}
		}
	}
	else
	{
		slave_poll_and_act();
	}
}

