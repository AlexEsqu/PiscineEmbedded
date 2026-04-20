#include "libalex_avr.h"

// Retourne 1 si master, 0 si slave
uint8_t negotiate_role(void)
{
    // Seed pseudo-random basé sur Timer1 au boot
    // Les 2 boards n'ont pas booté exactement au même moment
    // et TCNT1 tourne à 4µs/tick → valeur différente
    uint8_t seed = TCNT1 & 0x7F;   // 0..127
    uint8_t attempt = 0;

    while (1)
    {
        // === Phase 1 : on écoute pendant un délai random ===
        twi_slave_setup(PEER_ADDR);

        uint16_t listen_ms = 100 + (seed * 2) + (attempt * 50);   // 50..177 ms
        if (listen_ms > 1000)
            listen_ms = 1000; // capped at 1 second

        for (uint16_t t = 0; t < listen_ms; t++)
        {
            uint8_t data = 0;
            uint8_t st = twi_slave_poll(&data);

            if (st == 0x80 && data == MSG_HELLO)
            {
                return 0; // Quelqu'un nous a dit hello → on est slave
            }
            _delay_ms(1);
        }

        // === Phase 2 : on tente d'être master ===
        // Sortie du mode slave (disable TWEA)
        TWCR = (1 << TWEN);

        uint8_t ok = (twi_start() == 0x08)
                  && (twi_write(PEER_ADDR << 1) == 0x18)
                  && (twi_write(MSG_HELLO) == 0x28);
        twi_stop();

        if (ok) {
            return 1;   // master
        }

        // Personne en face ou collision → retente avec seed différent
        seed = (seed * 13 + 37) & 0x7F;
        attempt++;
    }
}