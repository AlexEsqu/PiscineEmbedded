#ifndef LIBALEX_AVR
# define LIBALEX_AVR

# include <avr/io.h>

# define BAUDRATE 115200


// TIMERS

// Table 16-4 Waveform Generation Mode p. 141
typedef enum {
    TIMER_MODE_NORMAL,
    TIMER_MODE_CTC,
    TIMER_MODE_PWM_PC,
    TIMER_MODE_PWM_PF,
    TIMER_MODE_FAST_PWM
} e_timer_mode;

// Table 16-4 Waveform Generation Mode p. 141 (cont)
typedef enum {
    TOP_DEFAULT,
    TOP_OCR1A,
    TOP_ICR1
} e_timer_submode;

// Table 16-5 Clock Select p.143
typedef enum
{
    CLK_STOP            = 0b000,
    CLK_DIV1            = 0b001,
    CLK_DIV8            = 0b010,
    CLK_DIV64           = 0b011,
    CLK_DIV256          = 0b100,
    CLK_DIV1024         = 0b101,
    CLK_EXT_FALLING     = 0b110,
    CLK_EXT_RISING      = 0b111
} e_divider;

// Table 16-1 and 16-2 Compare Output Mode p.140-141
typedef enum {
    CMP_DISCONNECT      = 0b00,
    CMP_TOGGLE          = 0b01,
    CMP_CLEAR           = 0b10,
    CMP_SET             = 0b11 
} e_compare_mode;


void timer_init(e_timer_mode mode, e_timer_submode submode, e_compare_mode comA, e_compare_mode comB);


// UART

void uart_init();

void uart_tx(char c);
void uart_printstr(const char* str);











#endif