#include "libalex_avr.h"

// • Vous devez écrire un programme qui :
// 	◦ chaque fois que vous pressez le bouton SW1 incrémente une valeur ;
// 	◦ chaque fois que vous pressez le bouton SW2 décrémente une valeur ;
// 	◦ et l’affiche en permanence cette valeur sur les LEDs D1 D2 D3 et D4 en binaire.
// • Vous devez utiliser les interrupts et ne rien avoir dans votre boucle de main.

volatile uint8_t value = 0;

#define DEBOUNCE_COUNT 200

// Per schema:
// LED D1

void displayNumber(int num)
{
    // resetting all the LEDS to 0, leaving the rest of PORTB alone
    PORTB &= 0b11101000;

    // explicitly extracting all binary digit of the count
    // for those like me not really up to spec in binary
	uint8_t numOnlyFourBit = (uint8_t)num & 0x0F;

    unsigned char binary1 =  (numOnlyFourBit & 0b00000001);
    unsigned char binary2 =  (numOnlyFourBit & 0b00000010);
    unsigned char binary4 =  (numOnlyFourBit & 0b00000100);
    // binary8 is shifted because it'll be placed a lil awkwardly
    // at index 4 instead of 3
    unsigned char binary8 = ((numOnlyFourBit & 0b00001000) << 1);

    // setting the LEDS to their respective values
    PORTB |= binary1 | binary2 | binary4 | binary8;
}

// Launch fast timer to check the button press actually was a button press
void	launchDebounceOnSwt1()
{
	// set COMA value to when debounce is over
	OCR1A = TCNT1 + DEBOUNCE_COUNT;

	// enable COMA interrupt on timer
	TIMSK1 |= (1 << OCIE1A);

	removeInterruptOnSwt1();
	EICRA ^= (1 << ISC00);
}

// Launch fast timer to check the button press actually was a button press
void	launchDebounceOnSwt2()
{
	// set COMB value to when debounce is over
	OCR1B = TCNT1 + DEBOUNCE_COUNT;

	// enable COMA interrupt on timer
	TIMSK1 |= (1 << OCIE1B);

	removeInterruptOnSwt2();

}

void	stopDebounceOnSwt1()
{
	setInterruptOnSwt1(); // re-enable interrupt on button

	// remove COMA interrupt on timer
	TIMSK1 &= ~(1 << OCIE1A);
}

void	stopDebounceOnSwt2()
{
	setInterruptOnSwt2(); // re-enable interrupt on button

	// remove COMB interrupt on timer
	TIMSK1 &= ~(1 << OCIE1B);
}

void	concludeDebounceOnSwt2()
{
	stopDebounceOnSwt2();

	if (PIND & (1 << PD4))	// if button is not still pressed, was probably faulty
		return;

	if (value <= 0)
		value = 15;
	else
		value--;

	displayNumber(value);

	while (!(PIND & (1 << PD4)))
		;
}

void	concludeDebounceOnSwt1()
{
	stopDebounceOnSwt1();

	if (PIND & (1 << PD2))	// if button is not still pressed, was probably faulty
		return;

	if (value >= 15)
		value = 0;
	else
		value++;

	displayNumber(value);

	while (!(PIND & (1 << PD2)))
		;

}

// Set the interrupt function of the External Interrupt Request 0
// Per datasheet Table 12-6 p. 77
void __attribute__((signal)) __vector_1 (void)
{
	launchDebounceOnSwt1();
}

// Set the interrupt function of the External Interrupt Request 0
// Per datasheet Table 12-6 p. 77
void __attribute__((signal)) __vector_5 (void)
{
	launchDebounceOnSwt2();
}

// Set the interrupt function of the Timer1 COMA
// Per datasheet Table 12-6 p. 77
void __attribute__((signal)) __vector_11 (void)
{
	concludeDebounceOnSwt1();
}


// Set the interrupt function of the Timer1 COMB
// Per datasheet Table 12-6 p. 77
void __attribute__((signal)) __vector_12 (void)
{
	concludeDebounceOnSwt2();
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

	// Set swt1 Interrupt to falling edge
	EICRA ^= (1 << ISC00);

	// Set External Interrupt Mask Register to allow for interrupts on SW1 and SW2
	// See datasheet p. 81
	setInterruptOnSwt1();

	// Set Pin Change Interrupt Control Register + Pin Change Mask Register 2
	// See datasheet p.82
	setInterruptOnSwt2();

	// DEBOUNCE INIT

	// Set timer1 to be used for debouncing the switches
	timer1_init(TIMER_MODE_CTC,TOP_DEFAULT,CMP_DISCONNECT,CMP_DISCONNECT);

	timer1_launch(CLK_DIV1024);

	while (1)
	{
		;
	}
}
