#include "libalex_avr.h"

// UTILS

// better be actually in the datasheet cuz otherwise will produce garbage
static int timer1_get_wgm(e_timer_mode mode, e_timer_submode sub)
{
    switch (mode)
    {
        case TIMER_MODE_NORMAL:
            return 0x0;
        case TIMER_MODE_CTC:
            if (sub == TOP_OCRA)
                return 0x4;
            if (sub == TOP_ICR)
                return 0xC;
            break;
        case TIMER_MODE_FAST_PWM:
            if (sub == TOP_ICR)
                return 0xE;
            if (sub == TOP_OCRA)
                return 0xF;
            break;
        case TIMER_MODE_PWM_PC:
            if (sub == TOP_ICR)
                return 0xA;
            if (sub == TOP_OCRA)
                return 0xB;
            break;
        case TIMER_MODE_PWM_PF:
            if (sub == TOP_ICR)
                return 0x8;
            if (sub == TOP_OCRA)
                return 0x9;
            break;
    }
    return 0x0;
}

// Per datasheet Table 15-8 p. 115
static int timer0_get_wgm(e_timer_mode mode, e_timer_submode sub)
{
    switch (mode)
    {
        case TIMER_MODE_NORMAL:
            return 0b000;
        case TIMER_MODE_CTC:
            return 0b010;
        case TIMER_MODE_FAST_PWM:
            if (sub == TOP_DEFAULT)
                return 0b011;
            if (sub == TOP_OCRA)
                return 0b111;
            break;
        case TIMER_MODE_PWM_PC:
            if (sub == TOP_DEFAULT)
                return 0b001;
            if (sub == TOP_OCRA)
                return 0b101;
            break;
        default:
            break;
    }
    return 0x0;
}

// Per datasheet Table 18-8 p. 164... actually same as timer 0
static int timer2_get_wgm(e_timer_mode mode, e_timer_submode sub)
{
    switch (mode)
    {
        case TIMER_MODE_NORMAL:
            return 0b000;
        case TIMER_MODE_CTC:
            return 0b010;
        case TIMER_MODE_FAST_PWM:
            if (sub == TOP_DEFAULT)
                return 0b011;
            if (sub == TOP_OCRA)
                return 0b111;
            break;
        case TIMER_MODE_PWM_PC:
            if (sub == TOP_DEFAULT)
                return 0b001;
            if (sub == TOP_OCRA)
                return 0b101;
            break;
        default:
            break;
    }
    return 0x0;
}


/*------------------------------*/
/*          TIMER 1             */
/*------------------------------*/

// View of the Timer1 Registers:

// TCCR1A :
// COM1A1	COM1A0	COM1B1	COM1B0	-----	-----	WGM11	WGM10
//		1		0		1		0		0		0		1		0

// TCCR1B :
// ICNC1	ICES1	-----	WGM13	WGM12	CS12	CS11	CS10
//		0		0		0		1		1		1		0		0

void timer1_init(e_timer_mode mode, e_timer_submode submode, e_compare_mode comA, e_compare_mode comB)
{
    char wgm = timer1_get_wgm(mode, submode);

    // extract the wgm bits, explicited here but should be directly into their spots
    char wgm10 =  wgm       & 0b1;
    char wgm11 = (wgm >> 1) & 0b1;
    char wgm12 = (wgm >> 2) & 0b1;
    char wgm13 = (wgm >> 3) & 0b1;

    char comA0 =  comA       & 0b1;
    char comA1 = (comA >> 1) & 0b1;

    char comB0 =  comB       & 0b1;
    char comB1 = (comB >> 1) & 0b1;

    // Initialize the timer (by default but in case previous set)
    TCNT1 = 0;

    TCCR1A = 0;
    TCCR1B = 0;

    // Set the wavelength generation mode in the two control registers
    TCCR1A |= (wgm10 << WGM10) | (wgm11 << WGM11);
    TCCR1B |= (wgm12 << WGM12) | (wgm13 << WGM13);

    // Set the compare value for COMA and B
    TCCR1A |= (comA0 << COM1A0) | (comA1 << COM1A1)
            | (comB0 << COM1B0) | (comB1 << COM1B1);
}

void timer1_launch(e_divider divider)
{
    TCCR1B = (TCCR1B & ~((1 << CS12) | (1 << CS11) | (1 << CS10)))
           | ((divider & 0x07) << CS10);
}


/*------------------------------*/
/*          TIMER 0             */
/*------------------------------*/

// View of the Timer0 Registers:

// TCCR0A :
// COM1A1	COM1A0	COM1B1	COM1B0	-----	-----	WGM01	WGM00
//		1		0		1		0		0		0		1		0

// TCCR0B :
// F0C0A	F0C0B	-----	-----	WGM02	CS02	CS01	CS00
//		0		0		0		1		1		1		0		0

void timer0_init(e_timer_mode mode, e_timer_submode submode, e_compare_mode comA, e_compare_mode comB)
{
    char wgm = timer0_get_wgm(mode, submode);

    // extract the wgm bits, explicited here but should be directly into their spots
    char wgm00 =  wgm       & 0b1;
    char wgm01 = (wgm >> 1) & 0b1;
    char wgm02 = (wgm >> 2) & 0b1;

    char comA0 =  comA       & 0b1;
    char comA1 = (comA >> 1) & 0b1;

    char comB0 =  comB       & 0b1;
    char comB1 = (comB >> 1) & 0b1;

    // Initialize the timer (by default but in case previous set)
    TCNT0 = 0;

    TCCR0A = 0;
    TCCR0B = 0;

    // Set the wavelength generation mode in the two control registers
    TCCR0A |= (wgm01 << WGM01) | (wgm00 << WGM00);
    TCCR0B |= (wgm02 << WGM02);

    // Set the compare value for COMA and B
    TCCR0A |= (comA0 << COM0A0) | (comA1 << COM0A1)
            | (comB0 << COM0B0) | (comB1 << COM0B1);

    // Set prediviser, which launches the timer
    TCCR0B |= (1 << CS02);
}

void timer0_launch(e_divider divider)
{
    TCCR0B = (TCCR0B & ~((1 << CS02) | (1 << CS01) | (1 << CS00)))
           | ((divider & 0x07) << CS00);
}


/*------------------------------*/
/*          TIMER 2             */
/*------------------------------*/

// View of the Timer2 Registers:

// TCCR0A :
// COM2A1	COM2A0	COM2B1	COM2B0	-----	-----	WGM21	WGM20
//		1		0		1		0		0		0		1		0

// TCCR0B :
// F0C2A	F0C2B	-----	-----	WGM22	CS22	CS21	CS20
//		0		0		0		1		1		1		0		0

void timer2_init(e_timer_mode mode, e_timer_submode submode, e_compare_mode comA, e_compare_mode comB)
{
    char wgm = timer2_get_wgm(mode, submode);

    // extract the wgm bits, explicited here but should be directly into their spots
    char wgm20 =  wgm       & 0b1;
    char wgm21 = (wgm >> 1) & 0b1;
    char wgm22 = (wgm >> 2) & 0b1;

    char comA0 =  comA       & 0b1;
    char comA1 = (comA >> 1) & 0b1;

    char comB0 =  comB       & 0b1;
    char comB1 = (comB >> 1) & 0b1;

    // Initialize the timer (by default but in case previous set)
    TCNT2 = 0;

    TCCR2A = 0;
    TCCR2B = 0;

    // Set the wavelength generation mode in the two control registers
    TCCR2A |= (wgm20 << WGM20) | (wgm21 << WGM21);
    TCCR2B |= (wgm22 << WGM22);

    // Set the compare value for COMA and B
    TCCR2A |= (comA0 << COM2A0) | (comA1 << COM2A1)
            | (comB0 << COM2B0) | (comB1 << COM2B1);

    // Set prediviser, which launches the timer
    TCCR2B |= (1 << CS22) | (1 << CS21);
}

void timer2_launch(e_divider divider)
{
    TCCR2B = (TCCR2B & ~((1 << CS22) | (1 << CS21) | (1 << CS20)))
           | ((divider & 0x07) << CS20);
}


/*------------------------------*/
/*          DELAY               */
/*------------------------------*/

void delay_ms(int ms)
{
	volatile unsigned long i = (F_CPU / 2) / 35 / 500;
    i *= ms;

	while (i)
	{
		i--;
	}
}






