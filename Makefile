LDFLAGS = -lncurses

SRCS = main.c snake_controls.c snake_gui.c utils.c
OBJS = $(SRCS:.c=.o)

TARGET = main.bin

# Default rule: Build the program
all: $(TARGET)

# Link object files into the final binary
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

main.o: main.c constants.h
snake_gui.o: snake_gui.c constants.h
snake_controls.o: snake_controls.c constants.h

# Compile each .c file into a .o file
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	rm -f $(OBJS) $(TARGET)