#include "avr/io.h"
#include "util/delay.h"

// You must write a program that:
//  ◦ increments a value each time you press button SW1
//  ◦ decrements a value each time you press button SW2
//  ◦ displays this value in binary on LEDs D1 D2 D3 and D4 permanently.
// You must use only the AVR registers ( DDRX, PORTX, PINX )

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
    unsigned char sw1_is_pressed = false;
    unsigned char sw2_is_pressed = false;
    unsigned char count = 0;

    while (1)
    {
        // if (!(SW1 == 0) && !(SW2 == 0))
        //     continue;
        
        // check if SW1 or SW2 are being pressed by reading bit at its address
        // source: ATmega328P p.101
        if (!sw1_is_pressed && (( PIND & (PIND << PD2)) == 0))
        {
            count++;
            sw1_is_pressed = true;
            while ((( PIND & (PIND << PD2)) == 0))
                _delay_ms(30);
        }
        
        if (sw1_is_pressed && !(( PIND & (PIND << PD2)) == 0)) 
        {
            sw1_is_pressed = false;
            while ((( PIND & (PIND << PD2)) == 0))
                _delay_ms(30);
        }

        if (!sw2_is_pressed && ((PIND & (PIND << PD4)) == 0))
        {
            count--;
            sw2_is_pressed = true;
            while (((PIND & (PIND << PD4)) == 0))
                _delay_ms(30);
        }
        if (sw2_is_pressed && !((PIND & (PIND << PD4)) == 0))
        {
            sw2_is_pressed = false;
            while (((PIND & (PIND << PD4)) == 0))
                _delay_ms(30);
        }

        PORTB &= 0b11110000;

        unsigned char binary1 = (count & 0b00000001);
        unsigned char binary2 = (count & 0b00000010);
        unsigned char binary4 = (count & 0b00000100);
        unsigned char binary8 = ((count & 0b00001000) << 1);

        unsigned char lightLed = 0;
        lightLed |= binary1;
        lightLed |= binary2;
        lightLed |= binary4;
        lightLed |= binary8;

        PORTB |= lightLed;
    }
    
    return (0);
}