
OUTPUT_DIRECTORY = build

SRCS = main.c snake_controls.c snake_gui.c utils.c
OBJS = $(patsubst %.c, $(OUTPUT_DIRECTORY)/%.o, $(SRCS))

# TARGET = main.bin

# Default to Linux if PLATFORM_TARGET is not set
PLATFORM_TARGET ?= linux

ifeq ($(PLATFORM_TARGET), linux)
    SRCS += snake_io_linux.c
	LDFLAGS = -lncurses
	TARGET = $(OUTPUT_DIRECTORY)/snake.bin
else ifeq ($(PLATFORM_TARGET), windows)
    SRCS += snake_io_windows.c
	TARGET = $(OUTPUT_DIRECTORY)/snake.exe
else
    $(error Invalid PLATFORM_TARGET specified. Use PLATFORM_TARGET=linux or PLATFORM_TARGET=windows)
endif

# Default rule: Build the program
all: $(OUTPUT_DIRECTORY) $(TARGET)

# Create build directory if it doesn't exist
$(OUTPUT_DIRECTORY):
	mkdir -p build

# Compile each .c file into a .o file
$(OUTPUT_DIRECTORY)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into the final binary
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# main.o: main.c constants.h
# snake_gui.o: snake_gui.c constants.h
# snake_controls.o: snake_controls.c constants.h
# snake_io.o: snake_io.c constants.h

linux:
	@$(MAKE) PLATFORM_TARGET=linux all

windows:
	@$(MAKE) PLATFORM_TARGET=windows all
	
clean:
	rm -rf $(OUTPUT_DIRECTORY)