NAME = setup

SRC = main.c

HEX = $(patsubst %.c,%.hex,$(SRC))
BIN = $(patsubst %.c,%.bin,$(SRC))

CHIP = atmega328p
F_CPU = 16000000
OPEN_SERIAL = -P /dev/ttyUSB0
SET_PROGRAM = -c arduino
SET_BAUD = -b 115200
AVR_LIBC_DIR=/tmp/avr-libc

COMPILE_FLAGS = -mmcu=$(CHIP) -I$(AVR_LIBC_DIR)/include -Wall -Wextra -Werror -DF_CPU=$(F_CPU)UL

# $(AVR_LIBC_DIR)/.CACHETAG:
# 							rm -rf $(AVR_LIBC_DIR)
# 							git clone --depth=1 https://github.com/avrdudes/avr-libc/ $(AVR_LIBC_DIR)
# 							cd $(AVR_LIBC_DIR) && ./bootstrap
# 							cd $(AVR_LIBC_DIR) && ./configure --build=`./config.guess` --host=avr
# 							cd $(AVR_LIBC_DIR) && make -j
# 							touch $(AVR_LIBC_DIR)/.CACHETAG

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


# Compile into binary files
$(BIN):		$(SRC)
			$(COMPILER) $(COMPILE_FLAGS) $< -o $@

# Convert from binary to hex to transfer through USB
$(HEX):		$(BIN)
			$(CONVERT_TO_HEX) $< $@



