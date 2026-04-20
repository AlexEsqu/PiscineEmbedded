#include "libalex_avr.h"

// • Vous devez écrire un programme qui :
// 	◦ chaque fois que vous pressez le bouton SW1 incrémente une valeur ;
// 	◦ chaque fois que vous pressez le bouton SW2 décrémente une valeur ;
// 	◦ et l’affiche en permanence cette valeur sur les LEDs D1 D2 D3 et D4 en binaire.
// • Vous devez utiliser les interrupts et ne rien avoir dans votre boucle de main.

volatile uint8_t value = 0;
volatile uint8_t isDebouncingSw1 = false;
volatile uint8_t isDebouncingSw2 = false;

// Per schema:
// LED D1

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

// Set the interrupt function of the External Interrupt Request 0
// Per datasheet Table 12-6 p. 77
void __attribute__((signal)) __vector_1 (void)
{
	value++;
	displayNumber(value);
	isDebouncingSw1 = true;
	EIMSK &= ~(1 << INT0);
	TIFR0 |= (1 << OCF0A);
	TCNT0 = 0;
	timer0_launch(CLK_DIV1024);
}

// Set the interrupt function of the External Interrupt Request 1
// Per datasheet Table 12-6 p. 77
void __attribute__((signal)) __vector_2 (void)
{
	value--;
	displayNumber(value);
	isDebouncingSw2 = true;
	EIMSK &= ~(1 << INT1);
	TIFR0 |= (1 << OCF0A);
	TCNT0 = 0;
	timer0_launch(CLK_DIV1024);
}

void __attribute__((signal)) __vector_14 (void)
{
	if (isDebouncingSw1)
	{
		isDebouncingSw1 = false;
		EIMSK |= (1 << INT0);
	}
	if (isDebouncingSw2)
	{
		isDebouncingSw2 = false;
		EIMSK |= (1 << INT1);
	}
}


int main()
{
	// GPIO INIT

	// Set LED D2 as output
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
	PORTB &= 0b11101000;

	// Set buttons SW1 and SW2 to read data
	DDRD &= ~(1 << PD2);
	DDRD &= ~(1 << PD4);
	PORTD |= (1 << PD2) | (1 << PD4);

	// INTERRUPTS INIT

	// Set Status Register (SREG) to allow for interrupts
	// See Global Interrupt Enable at datasheet p. 20
	SREG |= (1 << 7);

	// Fine tune interrupt of switch 1 and 2 on fall edge (button press)
	EICRA = (EICRA & 0b1111'1100) | FALL_EDGE;
	EICRA = (EICRA & 0b1111'0011) | FALL_EDGE << 2;

	// Set External Interrupt Mask Register to allow for interrupts on SW1 and SW2
	// See datasheet p. 81
	EIMSK |= (1 << INT0);
	// FIND WHERE SW2 HAS ITS PIN

	// DEBOUNCE INIT

	// Set timer0 to count 10ms, to be used for debouncing the switches
	timer0_init(TIMER_MODE_CTC,TOP_OCRA,CMP_DISCONNECT,CMP_DISCONNECT);

	// set timer0 TOP to 10 ms
	OCR0A = ((F_CPU * 0.01) / 1024UL) - 1;

	// set timer0 to compare to the TOP value in OCR1A
	TIMSK0 |= (1 << OCIE0A);

	// timer0_launch(CLK_DIV1024);

	while (1)
	{
		;
	}
}
