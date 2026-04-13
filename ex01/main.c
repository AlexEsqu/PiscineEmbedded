#include "avr/io.h"

// You must write a program that turns on the LED D1 (PB0).
// You must use only AVR registers (DDRX, PORTX, PINX)

int main()
{
    PORTB = 1;
    while(1) ;
    return (0);
}