CC = "C:\\Users\\alim2790\\AppData\\Local\\Arduino15\\packages\\esp8266\\tools\\xtensa-lx106-elf-gcc\\3.0.0-newlib4.0.0-gnu23-48f7b08/bin/xtensa-lx106-elf-gcc"
CFLAGS = "-IC:\\Users\\alim2790\\AppData\\Local\\Arduino15\\packages\\esp8266\\hardware\\esp8266\\3.0.0/tools/sdk/include" "-IC:\\Users\\alim2790\\AppData\\Local\\Arduino15\\packages\\esp8266\\hardware\\esp8266\\3.0.0/tools/sdk/lwip2/include" "-IC:\\Users\\alim2790\\AppData\\Local\\Arduino15\\packages\\esp8266\\hardware\\esp8266\\3.0.0/tools/sdk/libc/xtensa-lx106-elf/include" -Os -g -O3 -Wpointer-arith -Wundef -Wl,-EL -nostdlib -mlongcalls -mtext-section-literals  -D__ets__ -DICACHE_FLASH
SDK_LINKER_PATH = "-LC:\\Users\\alim2790\\AppData\\Local\\Arduino15\\packages\\esp8266\\hardware\\esp8266\\3.0.0/tools/sdk/libc/xtensa-lx106-elf/lib" "-LC:\\Users\\alim2790\\AppData\\Local\\Arduino15\\packages\\esp8266\\hardware\\esp8266\\3.0.0/tools/sdk/lib" "-LC:\\Users\\alim2790\\AppData\\Local\\Arduino15\\packages\\esp8266\\hardware\\esp8266\\3.0.0/tools/sdk/lib/NONOSDK22x_190703" "-TC:\\Users\\alim2790\\AppData\\Local\\Arduino15\\packages\\esp8266\\hardware\\esp8266\\3.0.0/tools/sdk/ld/eagle.flash.1m64.ld" -Wl,--gc-sections -Wl,-wrap,system_restart_local -Wl,-wrap,spi_flash_read -lhal -lphy -lpp -lnet80211 -llwip2-536-feat -lwpa -lcrypto -lmain -lwps -lbearssl -lespnow -lsmartconfig -lairkiss -lwpa2 -lm -lc -lgcc
EXTRA_LDFLAGS = $(SDK_LINKER_PATH) -nostdlib
SERIAL_PORT = COM5
FLASH_MODE = dout
FLASH_SIZE = 1m

# This is the name of the executable we are going to create
EXE = main.bin

# This is the default target that is going to be built when we run 'make'
all: $(EXE) upload

# This is the directory where our source will be stored
SRC_DIR = src

# This is the list of all source files
SRC = $(wildcard src/**/*.c) $(wildcard src/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)

# This is the directory where our object files will be stored
OBJ_DIR = obj

# This is the list of object files that we need to create
OBJ = $(patsubst src/%.c, $(OBJ_DIR)/%.elf, $(SRC))

# This is the rule to upload our code to the esp
upload:
	esptool.py --port SERIAL_PORT write_flash -fm FLASH_MODE -fs FLASH_SIZE 0x00000 main.bin

# This is the rule to build the executable
$(EXE): $(OBJ)
	$(CC) $(EXTRA_LDFLAGS) $^ -o $@

# These are the rules to build the object files
$(OBJ_DIR)/%.elf: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/**/%.elf: src/**/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/**/**/%.elf: src/**/**/%.c
	$(CC) -c $(CFLAGS) $< -o $@

# This is the rule to delete the old executable before recompiling
clean:
	rm $(EXE)
