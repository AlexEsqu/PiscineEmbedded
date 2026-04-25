#ifndef LIBALEX_AVR
# define LIBALEX_AVR

# include <avr/io.h>
# include <util/delay.h>

# define BAUDRATE 115200

// COLOR

typedef enum
{
	LED_BLUE	= PD3,
	LED_RED		= PD5,
	LED_GREEN	= PD6
} e_color_led;

typedef enum
{
	RED			= 0b00100000,
	GREEN		= 0b01000000,
	BLUE		= 0b00001000,
	YELLOW		= 0b01100000,
	CYAN		= 0b01001000,
	MAGENTA		= 0b00101000,
	WHITE		= 0b01101000
}	e_color;

void    init_rgb();
void    set_rgb(uint8_t r, uint8_t g, uint8_t b);
uint8_t get_red(uint32_t color);
uint8_t get_green(uint32_t color);
uint8_t get_blue(uint32_t color);

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
    TOP_OCRA,
    TOP_ICR
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


void    timer0_init(e_timer_mode mode, e_timer_submode submode, e_compare_mode comA, e_compare_mode comB);
void    timer0_launch(e_divider divider);

void    timer1_init(e_timer_mode mode, e_timer_submode submode, e_compare_mode comA, e_compare_mode comB);
void    timer1_launch(e_divider divider);

void    timer2_init(e_timer_mode mode, e_timer_submode submode, e_compare_mode comA, e_compare_mode comB);
void    timer2_launch(e_divider divider);

void    delay_ms(int ms);

// UART

void    uart_init();
void    uart_tx(char c);
char    uart_rx();
void    uart_printhex(uint32_t c);
void    uart_printstr(const char* str);


// BITSHIFT

void    toggle(uint8_t targetBit, volatile uint8_t* reg);
void    set(uint8_t targetBit, volatile uint8_t* reg);
void    clear(uint8_t targetBit, volatile uint8_t* reg);


#endif
