#include "libalex_avr.h"

unsigned char EEPROM_read(unsigned int uiAddress)
{
	if (uiAddress > EEPROM_SIZE)
        return (0);
    
    /* Wait for completion of previous write */
	while(EECR & (1<<EEPE))
		;

	/* Set up address register */
	EEAR = uiAddress;

	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);

	/* Return data from Data Register */
	return EEDR;
}

void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
    if (uiAddress > EEPROM_SIZE)
        return;
    
    /* Wait for completion of previous write */
    while(EECR & (1<<EEPE))
        ;

    /* Set up address and Data Registers */
    EEAR = uiAddress;
    EEDR = ucData;

    /* Write logical one to EEMPE */
    EECR |= (1<<EEMPE);
    
    /* Start eeprom write by setting EEPE */
    EECR |= (1<<EEPE);
}

void printAddr(uint32_t num)
{
    const char hex[] = "0123456789abcdef";
    char buf[7];
    uint32_t i = 0;

    while (num > 0 && i < sizeof(buf))
    {
        buf[i++] = hex[num % 16];
        num /= 16;
    }

	while (i < sizeof(buf))
	{
		buf[i++] = '0';
	}

    while (i > 0)
    {
        uart_tx(buf[--i]);
    }
}

void printByte(unsigned char c)
{
    const char hex[] = "0123456789abcdef";
    char buf[2];
    long i = 0;

    if (c == 0)
    {
    	uart_tx('0');
		uart_tx('0');
        return;
    }

	if (c < 8)
		uart_tx('0');

    while (c > 0 && i < sizeof(buf))
    {
        buf[i++] = hex[c % 16];
        c /= 16;
    }

    while (i > 0)
    {
        uart_tx(buf[--i]);
    }
}