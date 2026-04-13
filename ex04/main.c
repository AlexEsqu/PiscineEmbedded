#include "avr/io.h"
#include "util/delay.h"

// You must write a program that:
//  ◦ increments a value each time you press button SW1
//  ◦ decrements a value each time you press button SW2
//  ◦ displays this value in binary on LEDs D1 D2 D3 and D4 permanently.
// You must use only the AVR registers ( DDRX, PORTX, PINX )

#define SW1 (PIND << PB2)
#define SW2 (PIND << PB4)

bool   isPressed(int buttonAddress)
{
    return (buttonAddress == 0);
}


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
    bool sw1_is_pressed = isPressed(SW1);
    bool sw2_is_pressed = isPressed(SW2);
    int i = 0;

    while (1)
    {
        // check if SW1 is being pressed by reading bit at its address (PIND - 3)
        // source: ATmega328P p.101
        if (!sw1_is_pressed && isPressed(SW1))
        {
            i++;
            sw1_is_pressed = true;
            _delay_ms(30);
        }
        if (sw1_is_pressed && !isPressed(SW1))
        {
            sw1_is_pressed = false;
            _delay_ms(30);
        }
        if (!sw2_is_pressed && isPressed(SW2))
        {
            i--;
            sw2_is_pressed = true;
            _delay_ms(30);
        }
        if (sw2_is_pressed && !isPressed(SW2))
        {
            sw2_is_pressed = false;
            _delay_ms(30);
        }

        {
            // toggle PORTB at place 0
            PORTB = PORTB ^ (1 << 0);

            // sleep some to avoid bounce 
            _delay_ms(30);
        }
    }
    
    return (0);
}