NAME = hope

SRC = main.c

HEX = $(patsubst %.c,%.hex,$(SRC))
BIN = $(patsubst %.c,%.bin,$(SRC))

CHIP = atmega328p
F_CPU = 16000000UL
OPEN_SERIAL = -P /dev/ttyUSB0
SET_PROGRAM = -c arduino
SET_BAUD = -b 115200

COMPILE_FLAGS = -mmcu=$(CHIP) -Wall -Wextra -Werror -DF_CPU=$(F_CPU)

COMPILER = avr-gcc
CONVERT_TO_HEX = avr-objcopy -O ihex
FLASH = avrdude $(SET_PROGRAM) -p $(CHIP) $(OPEN_SERIAL) $(SET_BAUD) -U flash:w:$(HEX):i


all :
			make hex
			make flash

hex:		$(HEX)

flash:		
			$(FLASH)

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
