#include "avr/io.h"

// You must write a program that turns on LED D1 (PB0) when button SW1 (PD2)
// is pressed.
// When the button is released, the LED turns off.
// You must use only AVR registers (DDRX, PORTX, PINX).

int main()
{
    // Per Devboard Schema:
    // PORTD 2 = button SW1
    // PORTB 0 = LED_D1

    while (1)
    {
        // check if SW1 is being pressed by reading bit at its address D3
        // Using PIN instead of PORT cuz reading instead of writing
        // source: ATmega328P p.101
        if ((PIND & 0b100) == 0)
        {
            // set PORTB 0 at 1
            PORTB |= 0b00000001;
        }
        else
        {
            // set PORTB 0 at 0
            PORTB &= 0b11111110;
        }
    }
    
    return (0);
}