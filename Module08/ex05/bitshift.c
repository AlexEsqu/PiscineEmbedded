#include "libalex_avr.h"


void toggle(uint8_t targetBit, volatile uint8_t* reg)
{
    *reg ^= (1 << targetBit);
}

void set(uint8_t targetBit, volatile uint8_t* reg)
{
    *reg |= (1 << targetBit);
}

void clear(uint8_t targetBit, volatile uint8_t* reg)
{
    *reg &= (~(1 << targetBit));
}