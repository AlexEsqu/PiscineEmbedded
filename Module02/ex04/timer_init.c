#include "libalex_avr.h"

// better be actually in the datasheet cuz otherwise will produce garbage
static int timer1_get_wgm(e_timer_mode mode, e_timer_submode sub)
{
    switch (mode) 
    {
        case TIMER_MODE_NORMAL: 
            return 0x0;
        case TIMER_MODE_CTC:
            if (sub == TOP_OCR1A) 
                return 0x4;
            if (sub == TOP_ICR1)  
                return 0xC;
            break;
        case TIMER_MODE_FAST_PWM:
            if (sub == TOP_ICR1)  
                return 0xE;
            if (sub == TOP_OCR1A) 
                return 0xF;
            break;
        case TIMER_MODE_PWM_PC:
            if (sub == TOP_ICR1)  
                return 0xA;
            if (sub == TOP_OCR1A) 
                return 0xB;
            break;
        case TIMER_MODE_PWM_PF:
            if (sub == TOP_ICR1)  
                return 0x8;
            if (sub == TOP_OCR1A) 
                return 0x9;
            break;
    }
    return 0x0;
}


void timer_init(e_timer_mode mode, e_timer_submode submode, e_compare_mode comA, e_compare_mode comB)
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


// View of the Timer1 Registers:

// TCCR1A :
// COM1A1	COM1A0	COM1B1	COM1B0	-----	-----	WGM11	WGM10
//		1		0		1		0		0		0		1		0

// TCCR1B :
// ICNC1	ICES1	-----	WGM13	WGM12	CS12	CS11	CS10
//		0		0		0		1		1		1		0		0

// TCCR1C not yet needed (?)