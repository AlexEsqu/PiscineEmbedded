#include "libalex_avr.h"

// View of the Two Wire registers and their use:

// TWCR - TWI Control Register
// 	TWINT		TWEA		TWSTA		TWSTO		TWWC		TWEN		----		TWIE
//	0			0			0			0			0			0			1			0
//	[finished]	[ack enabl]	[ START ]	[ STOP ]	[wr collis]	[TWI enabl]	[		]	[TWI interrupt]

// TWSR - TWI Status Register
// 	TWS7		TWS6		TWS5		TWS4		TWS3		-----		TWPS1		TWPS0
//	0			0			0			0			0			0			0			0
//	[TWI logic												]	[		]	[ prescaler				]

// TWDR - TWI Data Register
// 	TWD7		TWD6		TWD5		TWD4		TWD3		TWD2		TWD1		TWD0
//	0			0			0			0			0			1			1			0
//	[	DATA to be transmitted (Transmit mode) or received (Receive mode)							]

// TWBR - TWI Bit Rate Register
// 	TWBR7		TWBR6		TWBR5		TWBR4		TWBR3		TWBR2		TWBR1		TWBR0
//	[ Division factor for the bit rate generator													]

// TWAR - TWI Slave Address Register

// TWAMR - TWI Slave Address Mask Register

// EXAMPLE: Master reads 3 bytes
// M: START
// M: SLA+R → attend ACK
// S: ACK (0x40)
// S: byte1 → attend ACK du master
// M: ACK  (twi_read_ack)           ← "encore"
// S: byte2 → attend ACK
// M: ACK  (twi_read_ack)           ← "encore"
// S: byte3 → attend ACK
// M: NACK (twi_read_nack)          ← "terminé"
// M: STOP

// Ce que le slave répondra quand le master le lira
// À mettre à jour depuis le main quand l'état du jeu change
uint8_t slave_reply_byte = 0;

// Set Bit Rate Register to required settings
void	twi_init(void)
{
	// 100 kHz @ 16+ MHz, prescaler 1 -> TWBR = 72
	// Formula (datasheet 22.5.2): SCL = F_CPU / (16 + 2 * TWBR * prescaler);
	TWBR = 72;
	TWSR = 0;
	TWCR = (1 << TWEN);
}

// Master: send START. Returns status (expected: 0x08, 0x10)
uint8_t twi_start(void)
{
	// send a START notification through the TWI ports
	TWCR = (1 << TWINT)| (1 << TWSTA) | (1 << TWEN);

	// Wait for the TWI to finish the task
	while (!(TWCR & (1 << TWINT)))
		;

	return TWSR & 0xF8;
}

// Master: send stop, no status to expect
uint8_t	twi_stop(void)
{
	// Transmit STOP notification
	TWCR = (1<<TWINT)|(1<<TWEN) | (1<<TWSTO);
    // pas de wait TWINT après STOP, le hardware ne le lève pas
    // Wait until STOP has been executed on the bus
    while (TWCR & (1 << TWSTO))
        ;
	return TWSR & 0xF8;
}

// Master: writes a byte (adress or data) returns status
uint8_t twi_write(uint8_t byte)
{
	// writes the byte in data register
	TWDR = byte;
	// 
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	return TWSR & 0xF8;
}

// Master: read a byte and ACK (signals: I want another byte after)
// used when reading multiple bytes, for all bytes except the last
uint8_t	twi_read_ack(void)
{
	// TWEA=1 means we'll send ACK after receiving the byte
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while (!(TWCR & (1 << TWINT)));
	return TWDR; // byte is now in TWDR data register
}

// Master: read a byte and NACK (signals: "last byte, stop sending")
// User for the LAST byte of a read sequence (or if only 1 byte to read)
uint8_t	twi_read_nack(void)
{
	// No TWEA: we'll send NACK after receiving -> slave stops
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

// Slave: set address and start listening
void twi_slave_setup(uint8_t addr)
{
    TWAR = (addr << 1);                      // 7-bit address, bit 0 = general call off
    TWCR = (1 << TWEN) | (1 << TWEA);        // enable + auto-ACK
}

// Slave poll: returns 0 if nothing, otherwise the status code
// If data was received, it's in *data (only valid when return = 0x80 or 0x88)
// Slave Transmitter Mode datasheet 22.7.4.
uint8_t twi_slave_poll(uint8_t *data)
{
    if (!(TWCR & (1 << TWINT)))
        return 0;                            // nothing happened

    uint8_t status = TWSR & 0xF8;

	// Master wrote to us, data in TWR
    if (status == 0x80 || status == 0x88)
	{
        *data = TWDR;
    }
	// Master wants to read from us
	// 0xAB = own SLA+R received, ACK returned
	// 0xB0 - arbitration lost + SLA+R received
	else if (status == 0xA8 || status == 0xB0)
	{
		TWDR = slave_reply_byte; // must load our reply into TWDR
	}

    // Release the bus: clear TWINT, re-enable ACK for next transaction
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);

    return status;
}

uint8_t twi_master_send_msg(uint8_t msg)
{
    if (twi_start() != 0x08)
	{
		twi_stop();
		return 0;
	}
    if (twi_write(PEER_ADDR << 1) != 0x18)
	{
		twi_stop();
		return 0;
	}
    if (twi_write(msg) != 0x28)
	{
		twi_stop();
		return 0;
	}
    twi_stop();
    return 1;
}

uint8_t twi_master_read_slave(void)
{
    if (twi_start() != 0x08)
	{
		twi_stop();
		return 0xFF;
	}
    if (twi_write((PEER_ADDR << 1) | 1) != 0x40)
	{
		twi_stop();
		return 0xFF;
	}
    uint8_t v = twi_read_nack();
    twi_stop();
    return v;
}