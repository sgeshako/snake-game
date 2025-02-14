all: main.c
	gcc main.c utils.c -o main.bin -lncurses
clean:
	rm -f main.bin