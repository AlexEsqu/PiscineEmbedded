NAME = switchLed

SRC = 	bitshift.c \
	button.c \
	game.c \
	main.c \
	negociator.c \
	polling.c \
	rgb.c \
	state_handler.c \
	timer.c \
	twi.c \
	uart.c

INC_DIR = inc
OBJ_DIR = obj

BIN = $(OBJ_DIR)/$(NAME).bin
HEX = $(OBJ_DIR)/$(NAME).hex
ASM = $(OBJ_DIR)/$(NAME).asm

CHIP = atmega328p
F_CPU = 16000000UL
OPEN_SERIAL = -P /dev/ttyUSB0
OPEN_SECOND_SERIAL = -P /dev/ttyUSB1
SET_PROGRAM = -c arduino
SET_BAUD = -b 115200

COMPILER = avr-gcc
COMPILE_FLAGS = -mmcu=$(CHIP) -I$(INC_DIR) -Wall -Wextra -Werror -Os -DF_CPU=$(F_CPU)

CONVERT_TO_HEX = avr-objcopy -O ihex

FLASH = avrdude $(SET_PROGRAM) -p $(CHIP) $(OPEN_SERIAL) $(SET_BAUD) -U flash:w:$(HEX):i
FLASH_SECOND = avrdude $(SET_PROGRAM) -p $(CHIP) $(OPEN_SECOND_SERIAL) $(SET_BAUD) -U flash:w:$(HEX):i

all :		hex flash

hex:		$(HEX)

flash:
			$(FLASH)

flash_both:	hex
			$(FLASH)
			$(FLASH_SECOND)

asm:		$(ASM)

run:		all
			screen /dev/ttyUSB0 115200

clean:
			rm -rf $(OBJ_DIR)

re:			clean all


# Compile into binary file
$(BIN):		$(SRC)
			mkdir -p $(OBJ_DIR)
			$(COMPILER) $(COMPILE_FLAGS) $^ -o $@

# Convert from binary to hex to transfer through USB
$(HEX):		$(BIN)
			$(CONVERT_TO_HEX) $< $@

# Compile into assembler file
$(ASM): 	$(SRC)
			$(COMPILER) $(COMPILE_FLAGS) -S $^ -o $@
