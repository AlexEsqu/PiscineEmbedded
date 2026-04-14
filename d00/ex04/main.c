/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 21:47:52 by mkling            #+#    #+#             */
/*   Updated: 2026/04/14 09:40:47 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "avr/io.h"
#include "util/delay.h"

// You must write a program that:
//  ◦ increments a value each time you press button SW1
//  ◦ decrements a value each time you press button SW2
//  ◦ displays this value in binary on LEDs D1 D2 D3 and D4 permanently.
// You must use only the AVR registers ( DDRX, PORTX, PINX )

void displayNumber(int num)
{
    // resetting all the LEDS to 0, leaving the rest of PORTB alone
    PORTB &= 0b11101000;

    // explicitly extracting all binary digit of the count
    // for those like me not really up to spec in binary
    unsigned char binary1 =  (num & 0b00000001);
    unsigned char binary2 =  (num & 0b00000010);
    unsigned char binary4 =  (num & 0b00000100);
    // binary8 is shifted because it'll be placed a lil awkwardly
    // at index 4 instead of 3
    unsigned char binary8 = ((num & 0b00001000) << 1);

    // setting the LEDS to their respective values
    PORTB |= binary1 | binary2 | binary4 | binary8;
}

bool isBeingPressed(int buttonPort, int buttonAddress)
{
    return ((buttonPort & (buttonPort << buttonAddress)) == 0);
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

    DDRB |= 0b00010111;
    DDRD &= ~(1 << 2) | (1 << 4);
    PORTD |= (1 << 2) | (1 << 4);

    // Initializing the buttons to their current states
    unsigned char sw1_was_pressed = isBeingPressed(PIND, PD2);
    unsigned char sw2_was_pressed = isBeingPressed(PIND, PD4);
    unsigned char count = 0;

    while (1)
    {
        if (!isBeingPressed(PIND, PD2) && !isBeingPressed(PIND, PD4))
            continue;

        if (!sw1_was_pressed && isBeingPressed(PIND, PD2))
        {
            count++;
            sw1_was_pressed = true;
            while (isBeingPressed(PIND, PD2))
                _delay_ms(30);
        }
        if (sw1_was_pressed && !isBeingPressed(PIND, PD2))
        {
            sw1_was_pressed = false;
            while (isBeingPressed(PIND, PD2))
                _delay_ms(30);
        }

        if (!sw2_was_pressed && isBeingPressed(PIND, PD4))
        {
            count--;
            sw2_was_pressed = true;
            while (isBeingPressed(PIND, PD4))
                _delay_ms(30);
        }
        if (sw2_was_pressed && !isBeingPressed(PIND, PD4))
        {
            sw2_was_pressed = false;
            while (isBeingPressed(PIND, PD4))
                _delay_ms(30);
        }

        displayNumber(count);
    }

    return (0);
}
