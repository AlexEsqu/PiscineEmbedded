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
	LED_D1	= PB0,
	LED_D2	= PB1,
	LED_D3	= PB2,
	LED_D4	= PB4
} e_red_led;

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

void	init_rgb();
void	set_rgb(uint8_t r, uint8_t g, uint8_t b);
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
	CLK_STOP			= 0b000,
	CLK_DIV1			= 0b001,
	CLK_DIV8			= 0b010,
	CLK_DIV64		   = 0b011,
	CLK_DIV256		  = 0b100,
	CLK_DIV1024		 = 0b101,
	CLK_EXT_FALLING	 = 0b110,
	CLK_EXT_RISING	  = 0b111
} e_divider;

// Table 16-1 and 16-2 Compare Output Mode p.140-141
typedef enum {
	CMP_DISCONNECT	  = 0b00,
	CMP_TOGGLE		  = 0b01,
	CMP_CLEAR		   = 0b10,
	CMP_SET			 = 0b11
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
void	uart_printhex(long c);
void	uart_printstr(const char* str);
void	uart_itoa(long num);
void	uart_utoa(uint32_t num);


// BITSHIFT

void	toggle(uint8_t targetBit, volatile uint8_t* reg);
void	set(uint8_t targetBit, volatile uint8_t* reg);
void	clear(uint8_t targetBit, volatile uint8_t* reg);

// ADC

void		adc_init();
uint16_t	get_adc0_conv();
uint16_t	get_adc1_conv();
uint16_t	get_adc2_conv();
uint16_t	get_adc8_conv();

// I2C

uint8_t		getI2cStatusCode();
void		waitForI2cTransmission();
void		i2c_init(void);
void		i2c_start(void);
void		i2c_write(unsigned char data);
char		i2c_read(void);
char		i2c_read_and_stop(void);
void		i2c_stop(void);
void		i2c_renew_start(void);
void		i2c_enter_master_receiver();
void		i2c_enter_master_transmitter();

// EEPROM

#define EEPROM_SIZE 1024

typedef struct
{
	uint32_t	byteAddress;
	uint32_t	newValue;
} eeprom_balaylaka_t;

void		printByte(unsigned char c);
void		printAddr(uint32_t num);
unsigned char EEPROM_read(unsigned int uiAddress);
void		EEPROM_write(unsigned int uiAddress, unsigned char ucData);
void		hexdumpEEPROM();
void		hexdumpEEPROMWithModif(eeprom_balaylaka_t result);

// PROMPT

# define BUFFER_SIZE 56
# define MIN_CMD_LEN 6
# define MAX_CMD_LEN 14
# define MAX_ARG_LEN 34

typedef enum
{
	PROMPT,
	RECEIVE_COMMAND,
	VALIDATE_EXECUTE
} e_state;

typedef enum
{
	STATUS,
	SET_ID,
	SET_PRIO,
	SET_TAG,
	FACTORY_RESET,
	UNKNOWN
}	e_command;

typedef struct
{
	e_command	command;
	uint32_t	newId;
	uint16_t	newPriority;
	char		newTag[33];
} command_content_t;


typedef struct
{
	uint32_t	magicNumber;
	uint32_t	nodeId;
	uint16_t	priority;
	char		tag[33];
	uint16_t	integrityCheck;
} node_t;

void handleUserTyping(char* buffer, int* bufferIndex, e_state* state);
char	isValidFormat(char* buffer);
void	bzeroStr(char *str, int size);
int ft_strcmp(char* str1, char* str2);
char	isHexDigit(char c);
uint32_t ft_strlen(char* str);
void	*ft_memmove(void *dest, const void *src, uint16_t len);
uint32_t	ft_atou(const char *str);
int			ft_atoi(const char *str);
void	*ft_memset(void *ptr, int value, unsigned long len);

#endif
