NAME = setup

SRC = main.c

OBJ = $(patsubst %.c,%.o,$(SRC))
ELF = $(patsubst %.c,%.elf,$(SRC))
HEX = $(patsubst %.c,%.hex,$(SRC))
BIN = $(patsubst %.c,%.bin,$(SRC))

CHIP = atmega328p
F_CPU = 16000000
OPEN_SERIAL = -P /dev/ttyUSB0
SET_PROGRAM = -c arduino
SET_BAUD = -b 115200


COMPILE_FLAGS = -nostdlib -mmcu=$(CHIP) -Wall -Wextra -Werror -DF_CPU=$(F_CPU)
LINK_FLAGS = -nostdlib -mmcu=$(CHIP)

COMPILER = avr-gcc
CONVERT_TO_BIN = avr-objcopy -O binary
CONVERT_TO_HEX = avr-objcopy -I binary -O ihex
FLASH = avrdude $(SET_PROGRAM) -p $(CHIP) $(OPEN_SERIAL) $(SET_BAUD) -U flash:w:$<:i


all :		
			make hex
			make flash

hex:		$(HEX)

# Flash hex file to the arduino board
flash:		$(HEX)
			$(FLASH)

clean:
			rm -rf $(OBJ)


# Compile into object files
$(OBJ):		$(SRC)
			$(COMPILER) $(COMPILE_FLAGS) -c $< -o $@

# Link into binary file
$(ELF):		$(OBJ)
			$(COMPILER) $(LINK_FLAGS) $^ -o $@

# Convert into bin cuz the subject said to
$(BIN):		$(ELF)
			$(CONVERT_TO_BIN) $< $@

# Convert from elf to hex to transfer through USB
$(HEX):		$(BIN)
			$(CONVERT_TO_HEX) $< $@



