CC = "C:\\Users\\alim2790\\AppData\\Local\\Arduino15\\packages\\esp8266\\tools\\xtensa-lx106-elf-gcc\\3.0.0-newlib4.0.0-gnu23-48f7b08/bin/xtensa-lx106-elf-gcc"
CFLAGS = -Isdk_includes
SDK_LINKER_PATH = "-LC:\\Users\\alim2790\\AppData\\Local\\Arduino15\\packages\\esp8266\\hardware\\esp8266\\3.0.0/tools/sdk/lib/NONOSDK22x_190703"
LDFLAGS = SDK_LINKER_PATH
SERIAL_PORT = COM5
FLASH_MODE = dout
FLASH_SIZE = 1m

# This is the name of the executable we are going to create
EXE = main.bin

# This is the default target that is going to be built when we run 'make'
all: clean $(EXE) upload

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
	ld $(LDFLAGS) $^ -o $@

# These are the rules to build the object files
$(OBJ_DIR)/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/**/%.o: src/**/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/**/**/%.o: src/**/**/%.c
	$(CC) -c $(CFLAGS) $< -o $@

# This is the rule to delete the old executable before recompiling
clean:
	rm -f $(EXE)