NAME = blinker

SRC = main.c

HEX = $(patsubst %.c,%.hex,$(SRC))
BIN = $(patsubst %.c,%.bin,$(SRC))
ASM = $(patsubst %.c,%.asm,$(SRC))

CHIP = atmega328p
F_CPU = 16000000UL
OPEN_SERIAL = -P /dev/ttyUSB0
SET_PROGRAM = -c arduino
SET_BAUD = -b 115200

COMPILE_FLAGS = -mmcu=$(CHIP) -Wall -Wextra -Werror -DF_CPU=$(F_CPU)

COMPILER = avr-gcc
CONVERT_TO_HEX = avr-objcopy -O ihex
FLASH = avrdude $(SET_PROGRAM) -p $(CHIP) $(OPEN_SERIAL) $(SET_BAUD) -U flash:w:$(HEX):i


all :		hex flash

hex:		$(HEX)

flash:
			$(FLASH)

asm:		$(ASM)

clean:
			rm -rf $(BIN)
			rm -rf $(HEX)

re:
			make clean
			make all

# Compile into binary file
$(BIN):		$(SRC)
			$(COMPILER) $(COMPILE_FLAGS) $< -o $@

# Convert from binary to hex to transfer through USB
$(HEX):		$(BIN)
			$(CONVERT_TO_HEX) $< $@

$(ASM): 	$(SRC)
			$(COMPILER) $(COMPILE_FLAGS) -S $< -o $@
