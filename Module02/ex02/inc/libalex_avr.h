#ifndef LIBALEX_AVR
# define LIBALEX_AVR

# include <avr/io.h>
# include <util/delay.h>

# define BAUDRATE 115200

void uart_init();
void uart_tx(char c);
char uart_rx();


#endif