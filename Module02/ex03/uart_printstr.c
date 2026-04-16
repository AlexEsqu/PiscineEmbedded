#include "libalex_avr.h"


void uart_printstr(const char* str)
{
    while (*str)
    {
        uart_tx(*str++);
    }
}