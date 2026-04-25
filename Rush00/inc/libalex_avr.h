#ifndef LIBALEX_AVR
# define LIBALEX_AVR

# include <avr/io.h>
# include <util/delay.h>
# include <stdbool.h>

# define BAUDRATE 115200

// =============================================================================
// PROTOCOLE APPLICATIF I2C
// =============================================================================
#define PEER_ADDR              0x42

#define MSG_HELLO              0xA0
#define MSG_GO_COUNTDOWN       0xA2
#define MSG_FALSE_START_ME     0xA4
#define MSG_FALSE_START_YOU    0xA5
#define MSG_YOU_WIN            0xA6
#define MSG_YOU_LOSE           0xA7

#define SLAVE_IDLE             0xB0
#define SLAVE_READY            0xB1
#define SLAVE_FALSE_START      0xB2
#define SLAVE_PRESSED_RACE     0xB3

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

void	setTimer0Timer2ForRGB();
void	set_rgb(uint8_t r, uint8_t g, uint8_t b);
uint8_t get_red(uint32_t color);
uint8_t get_green(uint32_t color);
uint8_t get_blue(uint32_t color);
void	setColorLedTo(e_color color);

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
	CLK_STOP			= 0b000,
	CLK_DIV1			= 0b001,
	CLK_DIV8			= 0b010,
	CLK_DIV64			= 0b011,
	CLK_DIV256			= 0b100,
	CLK_DIV1024			= 0b101,
	CLK_EXT_FALLING		= 0b110,
	CLK_EXT_RISING		= 0b111
} e_divider;

// Table 16-1 and 16-2 Compare Output Mode p.140-141
typedef enum {
	CMP_DISCONNECT		= 0b00,
	CMP_TOGGLE			= 0b01,
	CMP_CLEAR			= 0b10,
	CMP_SET				= 0b11
} e_compare_mode;


void	timer0_init(e_timer_mode mode, e_timer_submode submode, e_compare_mode comA, e_compare_mode comB);
void	timer0_launch(e_divider divider);

void	timer1_init(e_timer_mode mode, e_timer_submode submode, e_compare_mode comA, e_compare_mode comB);
void	timer1_launch(e_divider divider);

void	timer2_init(e_timer_mode mode, e_timer_submode submode, e_compare_mode comA, e_compare_mode comB);
void	timer2_launch(e_divider divider);

void	delay_ms(int ms);

// UART

void	uart_init();
void	uart_tx(char c);
char	uart_rx();
void	uart_printhex(uint32_t c);
void	uart_printstr(const char* str);


// BITSHIFT

void	toggle(uint8_t targetBit, volatile uint8_t* reg);
void	set(uint8_t targetBit, volatile uint8_t* reg);
void	clear(uint8_t targetBit, volatile uint8_t* reg);

// TWI

void        twi_init(void);
uint8_t     twi_start(void);
uint8_t	    twi_stop(void);
uint8_t     twi_write(uint8_t byte);
uint8_t     twi_slave_poll(uint8_t *data);
void        twi_slave_setup(uint8_t addr);
uint8_t	    twi_read_nack(void);
uint8_t	    twi_read_ack(void);
uint8_t 	twi_master_send_msg(uint8_t msg);
uint8_t 	twi_master_read_slave(void);


// RACE

typedef enum
{
	UNDECIDED,
	TYRANT,
	VICTIM
} e_control;

typedef enum
{
	START,					// program started, nothing received from other board
	FIRST_READY,			// has sent first comm, awaiting response from other player
	// LAST_READY,				// has received first comm from other board // @mchouikr - removed it bc no need if auto negociation in place
	COUNTDOWN,				// Start led countdown
	RACE,					// Await player button push
	WINNER,					// Display results
	LOSER,
	TIMEOUT,				// no player input in a minute
	ERROR,					// Problem, display orange led
	RENEGOTIATE,			// Peer reset detected, rejoin negotiation
} e_state;

typedef enum
{
	NO_ANIM,
	ANIM_WINNER,
	ANIM_LOSER
} e_animation;

// I2C

extern uint8_t slave_reply_byte;
extern uint8_t g_is_master;
extern volatile bool g_peer_ready;
extern volatile bool g_peer_pressed;

// NEGOCIATOR

uint8_t	 negotiate_role(void);

// STATE HANDLER

void	 handle_state_entry(e_state *s_prevState);
void     fight_for_control(void);

// POLLING

void twi_polling(void);

// GAME

extern volatile e_state g_state;			// which state of the game are we in
extern volatile uint8_t g_secondsInState;	// how many seconds have we been in that state
extern volatile e_control g_control;		// are we controller or controlled

// ANIMATION

extern volatile e_animation g_animation;	// which animation is ongoing on the RGB LED
extern volatile uint8_t g_anim_step;		// which step of the animation is ongoing
extern volatile uint8_t g_anim_ticks;		// which tick of the ongoing animation step
extern volatile bool g_anim_done;			// is the animation done?

extern const uint8_t c_ticks_per_anim_steps;// how many ticks should an animation step have
extern const uint8_t c_steps_per_anim;		// how many steps should an animation have

// DEBOUNCING

extern volatile uint8_t g_debounce_tick;	// which tick of the ongoing debouncing

extern const uint8_t c_max_debounce_tick;	// how many ticks a debounce last

// TIMER

extern volatile uint16_t g_timer_to_next_10ms;	// which compare value for OCR1B to ring timer in 10ms

extern const uint8_t c_ticks_to_reach_10ms;		// how many ticks at 16Mhz / 1024 to spend 10ms


void	startRace();
void	startCountdown();
void	startAnimation(e_animation anim);
void	displayErrorLED(void);
void	setGamer();
void	setDebounceTimer();
void	timeoutRace();
void	startRace();
void	setupTimer1BToRingIn10ms(uint16_t currentTimerTime);
void	setGeneralTimer();
void	removeInterruptOnSwt1();
void	setInterruptOnSwt1();


#endif
