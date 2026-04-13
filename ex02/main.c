#include "avr/io.h"

// You must write a program that turns on LED D1 (PB0) when button SW1 (PD2)
// is pressed.
// When the button is released, the LED turns off.
// You must use only AVR registers (DDRX, PORTX, PINX).

int main()
{
    // PORTD 2 = button
    // PORTB 0 = led

    while(1)
    {
        // check if PORT 2 is being pressed
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