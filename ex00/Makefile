NAME = setup

SRC = main.c

HEX = $(patsubst %.c,%.hex,$(SRC))
BIN = $(patsubst %.c,%.bin,$(SRC))

CHIP = atmega328p
F_CPU = 16000000
OPEN_SERIAL = -P /dev/ttyUSB0
SET_PROGRAM = -c arduino
SET_BAUD = -b 115200

COMPILE_FLAGS = -nostdlib -mmcu=$(CHIP) -Wall -Wextra -Werror -DF_CPU=$(F_CPU)UL
# LINK_FLAGS = -nostdlib -mmcu=$(CHIP)

COMPILER = avr-gcc 
CONVERT_TO_HEX = avr-objcopy -I binary -O ihex
FLASH = avrdude $(SET_PROGRAM) -p $(CHIP) $(OPEN_SERIAL) $(SET_BAUD) -U flash:w:$<:i


all :		
			make hex
			make flash

hex:		$(HEX)

flash:		$(HEX) 
			$(FLASH)

clean:
			rm -rf $(BIN)
			rm -rf $(HEX)


# Compile into binary file
$(BIN):		$(SRC)
			$(COMPILER) $(COMPILE_FLAGS) $< -o $@

# # Link into binary file
# $(ELF):		$(BIN)
# 			$(COMPILER) $(LINK_FLAGS) $^ -o $@

# Convert from binary to hex to transfer through USB
$(HEX):		$(BIN)
			$(CONVERT_TO_HEX) $< $@



