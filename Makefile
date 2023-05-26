CC = "C:\\Users\\alim2790\\AppData\\Local\\Arduino15\\packages\\esp8266\\tools\\xtensa-lx106-elf-gcc\\3.0.0-newlib4.0.0-gnu23-48f7b08/bin/xtensa-lx106-elf-gcc"
CFLAGS = "-IC:\\Users\\alim2790\\AppData\\Local\\Arduino15\\packages\\esp8266\\hardware\\esp8266\\3.0.0/tools/sdk/include" "-IC:\\Users\\alim2790\\AppData\\Local\\Arduino15\\packages\\esp8266\\hardware\\esp8266\\3.0.0/tools/sdk/lwip2/include" "-IC:\\Users\\alim2790\\AppData\\Local\\Arduino15\\packages\\esp8266\\hardware\\esp8266\\3.0.0/tools/sdk/libc/xtensa-lx106-elf/include" -Os -g -O3 -Wpointer-arith -Wundef -Wl,-EL -nostdlib -mlongcalls -mtext-section-literals  -D__ets__ -DICACHE_FLASH
SDK_LINKER_PATH = "-LC:\\Users\\alim2790\\AppData\\Local\\Arduino15\\packages\\esp8266\\hardware\\esp8266\\3.0.0/tools/sdk/libc/xtensa-lx106-elf/lib" "-LC:\\Users\\alim2790\\AppData\\Local\\Arduino15\\packages\\esp8266\\hardware\\esp8266\\3.0.0/tools/sdk/lib" "-LC:\\Users\\alim2790\\AppData\\Local\\Arduino15\\packages\\esp8266\\hardware\\esp8266\\3.0.0/tools/sdk/lib/NONOSDK22x_190703" -Wl,--gc-sections -Wl,-wrap,system_restart_local -Wl,-wrap,spi_flash_read -lhal -lphy -lpp -lnet80211 -llwip2-536-feat -lwpa -lcrypto -lmain -lwps -lbearssl -lespnow -lsmartconfig -lairkiss -lwpa2 -lm -lc -lgcc
EXTRA_LDFLAGS = $(SDK_LINKER_PATH) -nostdlib
SERIAL_PORT = COM5
BAUD_RATE = 9600
PYTHON = "C:\Users\alim2790\AppData\Local\Arduino15\packages\esp8266\tools\python3\3.7.2-post1/python3"
UPLOAD_SCRIPT = "C:\Users\alim2790\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.0.0/tools/upload.py"

# This is the name of the executable we are going to create
EXE = main.bin

# This is the default target that is going to be built when we run 'make'
all: $(EXE) upload

# This is the directory where our source will be stored
SRC_DIR = src

# This is the list of all source files
SRC = $(wildcard $(SRC_DIR)/*.c) $(foreach sdir, $(SRC_DIR), $(wildcard $(sdir)/*.c))

# This is the directory where our object files will be stored
OBJ_DIR = obj

# This is the list of object files that we need to create
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.elf, $(SRC))

# This is the rule to upload our code to the esp
upload:
	$(PYTHON) $(UPLOAD_SCRIPT) --chip esp8266 --port $(SERIAL_PORT) --baud $(BAUD_RATE) --before default_reset --after hard_reset write_flash 0x0 $(EXE)

# This is the rule to build the executable
$(EXE): $(OBJ)
	$(CC) $(EXTRA_LDFLAGS) $^ -o $@

# These are the rules to build the object files
$(OBJ_DIR)/%.elf: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/**/%.elf: $(SRC_DIR)/**/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/**/**/%.elf: $(SRC_DIR)/**/**/%.c
	$(CC) -c $(CFLAGS) $< -o $@

# This is the rule to delete the old executable before recompiling
clean:
	rm $(EXE)
