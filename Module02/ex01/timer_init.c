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
    wgm = timer1_get_wgm(mode, submode);

    wgm10 =  wgm       & 0x01;
    wgm11 = (wgm >> 1) & 0x01;
    wgm12 = (wgm >> 2) & 0x01;
    wgm13 = (wgm >> 3) & 0x01;

    // Initialize the timer (by default but in case previous set)
    TCNT1 = 0;

    // Set the wavelength generation mode (wgm) in the two control registers
    TCCR1A |= (wgm10 << WGM10) | (wgm11 << WGM11);
    TCCR1B |= (wgm12 << WGM12) | (wgm13 << WGM13);
}