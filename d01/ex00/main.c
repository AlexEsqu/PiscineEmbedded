#include "avr/io.h"
#include "util/delay.h"

// • You are required to write a program that turns on and off LED D2 (PB1) at a
// frequency of 1Hz with a variable duty cycle going from 10 to 100%.
// • You must configure the registers of Timer1 to control the LED.
// • The duty cycle must be managed as follow:
//     ◦ Pressing button SW1 increments the duty cycle by 10 %.
//     ◦ Pressing button SW2 decrements the duty cycle by 10 %

void T1Delay();

int main()
{
    // initializing LED D2 (PB1)as output and set at 0
    DDRB =|  0b00000010;
    PORTB =& 0b11111101;

    while (1)
    {
        TCNT1 = 0x1FF;
    }
}

// using the Timer1 per 
// source: ATmega328P p.121 - 123
void T1Delay()
{
    TCNT1 = 0x1FF;
}