#include "libalex_avr.h"

void     fight_for_control(void)
{
    // --- NÉGOCIATION MASTER/SLAVE (avant les interrupts timers) ---
    g_is_master = negotiate_role();

    if (!g_is_master) {
        slave_reply_byte = SLAVE_IDLE;
    }
}

void handle_state_entry(e_state *s_prevState)
{
    e_state cur = g_state;

    // === ENTRY ACTIONS (quand l'état change) ===
    if (cur != *s_prevState)
    {
        switch (cur)
        {
            case START:
                if (!g_is_master)
                {
                    set_rgb(0x02, 0x0, 0x0);
                    slave_reply_byte = SLAVE_IDLE;
                }
                else
                    set_rgb(0x00, 0x02, 0x0);
                break;
            case FIRST_READY:
                // Bouton local pressé, en attente du peer
                set_rgb(0x10, 0x10, 0x00);   // jaune
                if (!g_is_master)
                {
                    slave_reply_byte = SLAVE_READY;
                }
                break;
            case COUNTDOWN:
                // startCountdown() est appelé dans twi_polling au moment du MSG_GO_COUNTDOWN
                // ou dans slave_poll_and_act côté slave
                // Pas besoin de le rappeler ici
                break;
            case RACE:
                startRace();
                break;
            case WINNER:
                // Si on est master et qu'on a vu le slave appuyer d'abord,
                // ce sera MSG_YOU_WIN qui met WINNER chez lui.
                // Si on est arrivé ici par l'ISR du bouton en RACE,
                // il faut avertir le peer
                if (g_is_master)
                {
                    // Seulement si pas déjà fait par le slave_poll
                    twi_master_send_msg(MSG_YOU_LOSE);   // peer a perdu
                }
                else if (*s_prevState == RACE)
                {
                    // Slave a appuyé pendant la race → signale au master
                    slave_reply_byte = SLAVE_PRESSED_RACE;
                }
                else
                {
                    // Slave arrivé en WINNER via message I2C (MSG_YOU_WIN ou FALSE_START_ME)
                    slave_reply_byte = SLAVE_IDLE;
                }
                startAnimation(ANIM_WINNER);
                break;
            case LOSER:
                if (g_is_master) {
                    twi_master_send_msg(MSG_YOU_WIN);    // peer a gagné
                }
                else if (*s_prevState == COUNTDOWN)
                {
                    // Slave a faux-départé → signale au master
                    slave_reply_byte = SLAVE_FALSE_START;
                }                
                startAnimation(ANIM_LOSER);
                break;
            case TIMEOUT:
                g_state = LOSER;
                // Ne pas mettre à jour s_prevState: la transition LOSER sera
                // traitée au prochain appel                
                break;
            case RENEGOTIATE:
                // Éteindre tout ce qui tourne
                PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4));
                g_animation = NO_ANIM;
                g_anim_done = false;
                set_rgb(0x00, 0x00, 0x00);
                
                // Bloquant, jusqu'à ce qu'on retrouve un rôle
                g_is_master = negotiate_role();
                
                if (!g_is_master) {
                    slave_reply_byte = SLAVE_IDLE;
                }
                else {
                    break;
                }
                g_state = START;
                // Pareil que TIMEOUT: laisse la transition START se traiter
                // au prochain tour            
                break;    
            default:
                break;
        }
        *s_prevState = cur;
    }
}