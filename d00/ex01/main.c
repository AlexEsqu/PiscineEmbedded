/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@learner.42.tech>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 21:47:16 by mkling            #+#    #+#             */
/*   Updated: 2026/04/13 21:47:21 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "avr/io.h"

// You must write a program that turns on the LED D1 (PB0).
// You must use only AVR registers (DDRX, PORTX, PINX)

int main()
{
    // Per Devboard Schema:
    // PORTB 0 = LED_D1

    // Per Yingbo, need to initialize at 0
    // Gives wonderful  fuller color to LEDs
    DDRB |= 1;

    // set PORTB at 0x00000001
    PORTB |= 0b00000001;

    // loop to ensure it stays on (brutish I know)
    while (1) ;

    return (0);
}
