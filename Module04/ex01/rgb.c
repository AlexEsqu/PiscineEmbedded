#include "libalex_avr.h"

// Per datasheet Table 14-9 Alternate Function of Port D
// The PD6, PD5 and PD3 ports can be toggled by :
// - G - PD6: 0C0A (Timer Counter 0 Output Match A)
// - R - PD5: 0C0B (Timer Counter 0 Output Match B)
// - B - PD3: 0C2B (Timer Counter 2 Output Match B)
// So setting the TOP of the correct timers to the appropriate values
// and initializing the correct timers

void init_rgb()
{
    // setting portD LEDs as output
    DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);

    // initializing timer 0 and 2
    timer0_init(TIMER_MODE_FAST_PWM, TOP_DEFAULT, CMP_SET, CMP_SET);
    timer2_init(TIMER_MODE_FAST_PWM, TOP_DEFAULT, CMP_DISCONNECT, CMP_SET);
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    OCR0A = 255 - g;
    OCR0B = 255 - r;
    OCR2B = 255 - b;
}

uint8_t get_red(uint32_t color)
{
    return (uint8_t)((color >> 16) & 0xFF);
}

uint8_t get_green(uint32_t color)
{
    return (uint8_t)((color >> 8) & 0xFF);
}

uint8_t get_blue(uint32_t color)
{
    return (uint8_t)(color & 0xFF);
}
