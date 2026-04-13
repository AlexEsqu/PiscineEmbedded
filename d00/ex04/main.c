#include "avr/io.h"
#include "util/delay.h"

// You must write a program that:
//  ◦ increments a value each time you press button SW1
//  ◦ decrements a value each time you press button SW2
//  ◦ displays this value in binary on LEDs D1 D2 D3 and D4 permanently.
// You must use only the AVR registers ( DDRX, PORTX, PINX )

#define SW1 (PIND & (PIND << PD2))
#define SW2 (PIND & (PIND << PD4))

int main()
{
    // Per Devboard Schema:
    // PORTD 2 = button SW1
    // PORTD 4 = button SW2
    // PORTB 0 = LED_D1
    // PORTB 1 = LED_D2
    // PORTB 2 = LED_D3
    // PORTB 4 = LED_D4

    // Initializing the buttons to their current states
    unsigned char sw1_is_pressed = (SW1 == 0);
    unsigned char sw2_is_pressed = (SW2 == 0);
    unsigned char count = 0;

    while (1)
    {
        if (!(SW1 == 0) && !(SW2 == 0))
            continue;

        if (!sw1_is_pressed && (SW1 == 0))
        {
            count++;
            sw1_is_pressed = true;
            while ((SW1 == 0))
                _delay_ms(30);
        }
        if (sw1_is_pressed && !(SW1 == 0))
        {
            sw1_is_pressed = false;
            while ((SW1 == 0))
                _delay_ms(30);
        }

        if (!sw2_is_pressed && (SW2 == 0))
        {
            count--;
            sw2_is_pressed = true;
            while ((SW2 == 0))
                _delay_ms(30);
        }
        if (sw2_is_pressed && !(SW2 == 0))
        {
            sw2_is_pressed = false;
            while ((SW2 == 0))
                _delay_ms(30);
        }

        // resetting all the LEDS to 0, leaving the rest of PORTB alone
        PORTB &= 0b11101000;

        // explicitly extracting all binary digit of the count
        // for those like me not really up to spec in binary
        unsigned char binary1 =  (count & 0b00000001);
        unsigned char binary2 =  (count & 0b00000010);
        unsigned char binary4 =  (count & 0b00000100);
        // binary8 is shifted because it'll be placed a lil awkwardly
        // at index 4 instead of 3
        unsigned char binary8 = ((count & 0b00001000) << 1);

        // setting the LEDS to their respective values
        PORTB |= binary1 | binary2 | binary4 | binary8;
    }

    return (0);
}
