NAME = sniper

SRC = 	main.c \
		uart.c \
		bitshift.c \
		timer.c \
		adc.c \
		i2c.c \
		eeprom.c \
		rgb.c

INC_DIR = inc
OBJ_DIR = obj

BIN = $(OBJ_DIR)/$(NAME).bin
HEX = $(OBJ_DIR)/$(NAME).hex
ASM = $(OBJ_DIR)/$(NAME).asm

CHIP = atmega328p
F_CPU = 16000000UL
OPEN_SERIAL = -P /dev/ttyUSB0
SET_PROGRAM = -c arduino
SET_BAUD = -b 115200
# AVR_LIBC_DIR=/tmp/avr-libc

# $(AVR_LIBC_DIR)/.CACHETAG:
# 							rm -rf $(AVR_LIBC_DIR)
# 							git clone --depth=1 https://github.com/avrdudes/avr-libc/ $(AVR_LIBC_DIR)
# 							cd $(AVR_LIBC_DIR) && ./bootstrap
# 							cd $(AVR_LIBC_DIR) && ./configure --build=`./config.guess` --host=avr
# 							cd $(AVR_LIBC_DIR) && make -j
# 							touch $(AVR_LIBC_DIR)/.CACHETAG

COMPILER = avr-gcc
COMPILE_FLAGS = -mmcu=$(CHIP) -I$(INC_DIR) -Os -Wall -Wextra -Werror -DF_CPU=$(F_CPU)

CONVERT_TO_HEX = avr-objcopy -O ihex

FLASH = avrdude $(SET_PROGRAM) -p $(CHIP) $(OPEN_SERIAL) $(SET_BAUD) -U flash:w:$(HEX):i


all :		hex flash

hex:		$(HEX)

flash:
			$(FLASH)

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
