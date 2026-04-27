EEPROM = electrically erasable programmable read-only memory, a type of non-volatile memory, usually integrated in microcontrollers or as a separate chip device to store small amounts of data, allowing individual bytes to be erased and reprogrammed.


	PORTB &= 0b11111110
== 	PORTB &= ~0b00000001;
== 	PORTB &= ~(1 << PB0);

# TIMER1

## Components

3 16-bit Registers:
- **Timer/Counter** (TCNT1), value referred to as `n`

- **Output Compare Register** (OCR1A + OCR1B), value referred to as `x`
	- Output Compare Pin (OC1A/B)
	- Compare Match Flag (OCF1A/B)

- **Inpute Capture Register** (ICR1)
	- Input Capture Pin (ICP1)
	- Analog Comparator Pin

Must use precise form for accessing Counter values if using register or bit defines
Precise Form =

## Definition

BOTTOM = 0x0000
MAX = 0xFFFF
TOP = Either 0x00FF, 0x01FF, 0x03FF, or value in OCR1A or ICR1 register, depending on mode of operation

### Prescaler
Electronic counting circuit used to reduce a high frequency electrical signal to a lower frequency by integer division ; allows timer to be clocked at rates different from the clock ; synonym to counter-divider

More Info (FR): https://passionelectronique.fr/introduction-timer-arduino/

## Accessing 16-bit Registers
In ASM, 16-bit register must be accessed using :
- WRITE: high byte, then low byte
- READ: low byte, then high byte

Accessing the 16bit register is an atomic operation => must disable interrupts

## Clock Sources

Timer/Counter can be clocked by internal or external clock sources, as selected by the Clock Select (CS12:0) bit in the Timer/Counter controle Register B (TCCR1B)

## Counter Unit

- Counter High (TCNT1H)
- Counter Low (TCNT1L)




## Modes of Operations

Mode of the Timer/Counter is defined by combination of the Waveform Generation Mode (WGM13:0) and Compare Output Mode (COM1x1:0) bits


### Normal Mode
[WGM13:0 = 0]

Counter increments, no counter clear, overruns at MAX 0xFFFF then resterts from BOTTOM 0x0000
Timer/Counter Overflow Flag (TOV1) set same cycle as TCNT1 becomes 0, but never cleared


### Clear Timer on Compare Match (CTC) Mode

Counter increases until compare match, then counter is cleared

- Compare to OCR1A
Counter cleared to 0 when counter value (TCNT1) matches OCR1A, which then defines the TOP

- Compare to ICR1
Counter cleared to 0 when counter value (TCNT1) matches ICR1 , which then defines the TOP


### Fast Pulse Width Modulation (Fast PWD) Mode

Timer/Counter Overflow Flag (TOV1)	set every time counter reaches TOP, then counter clears
OC1A or ICF1 Flag 					set at the same time

### Phase Correct PWM Mode

Counter counts from BOTTOM to TOP then TOP to BOTTOM (double-slope operation)

- Non-Inverting	Compare Output Mode
OC1x is cleared on compare match with counter value while up  counting
OC1x is set		on compare match with counter value while downcounting

- Inverting		Compare Output Mode
OC1x is set		on compare match with counter value while up  counting
OC1x is cleared	on compare match with counter value while downcounting

### Phase and Frequency PWM Mode

Same as Phase correct but more precise when chaning the TOP value as counter is running





# Frequency calculations

OCR1A = \frac{F\_CPU}{2 \times prescaler \times blink\_frequency} - 1



WARNING assigning the prescaler launches the timer



# EEPROM

## EEPROM Data Memory in Datasheet (p.29 - )

### Preventing EEPROM Corruption
EEPROM data corruption during low Vcc because both write sequence and CPU execution require high enough voltage
=> Keep AVR RESET active (low) during insufficiant power supply voltage, by enabeing interna Brown out Detector (BOD)
