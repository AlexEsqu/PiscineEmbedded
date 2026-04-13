#include "avr/io.h"
#include "util/delay.h"

// You must write a program that reverses the state of the PB0 LED each time the
// SW1 button (PD2) changes from the released state to the pressed state.
// You must use only AVR registers ( DDRX, PORTX, PINX ).

int main()
{
    // Per Devboard Schema:
    // PORTD 2 = button SW1
    // PORTB 0 = LED_D1

    bool is_pressed = false;

    while (1)
    {
        // check if SW1 is being pressed by reading bit at its address (PIND - 3)
        // source: ATmega328P p.101
        if (!is_pressed && ((PIND & 0b100) == 0))
        {
            // toggle PORTB at place 0
            PORTB = PORTB ^ (1 << 0);

            // sleep some to avoid bounce 
            while (((PIND & 0b100) == 0))
                _delay_ms(30);

            is_pressed = true;
        }
        if (is_pressed && !((PIND & 0b100) == 0))
        {
            // sleep some to avoid bounce 
            while (((PIND & 0b100) == 0))
                _delay_ms(30);

            is_pressed = false;
        }
    }
    
    return (0);
}