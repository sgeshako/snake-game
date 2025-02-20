#include "snake_io.h"
#include <ncurses.h>
#include <time.h>
#include "constants.h"
#include "utils.h"

static int input_timeout_ms = -1;

void snake_io_init(int timeout_ms)
{
    // Initialize ncurses
    initscr();              // Start curses mode
    // cbreak();               // Disable line buffering
    // noecho();               // Don't echo input
    keypad(stdscr, TRUE);   // Enable special keys (e.g., arrow keys)
    timeout(timeout_ms);           // Wait for input for 100ms, then return ERR
    
    input_timeout_ms = timeout_ms;
}

void snake_io_exit()
{
    endwin(); // End curses mode
}

int wait_for_input()
{
    int key;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    long elapsed_ms = 0;

    // Try to get a key press blocking for 100ms
    key = getch();
    
    // if early, wait for the remaining ms 
    while (elapsed_ms < input_timeout_ms)
    {
        struct timespec sleep_time;
        sleep_time.tv_sec = 0;
        sleep_time.tv_nsec = 25 * 1000000;
        // sleep in 25ms chunks
        nanosleep(&sleep_time, NULL);

        clock_gettime(CLOCK_MONOTONIC, &end);
        elapsed_ms = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
    }

    if (key == ERR)
    {
        return NO_INPUT;
    }
    
    return key;
}

static e_direction map_key_to_direction(int key_code)
{
    static const int key_map[] = {
        KEY_LEFT,   // 0: LEFT  
        KEY_RIGHT,  // 1: RIGHT 
        KEY_UP,     // 3: TOP   
        KEY_DOWN    // 4: DOWN  
    };

    return index_of(key_map, sizeof(key_map) / sizeof(key_map[0]), key_code);    
}

int normalize_input(int key_code)
{
    e_direction direction = map_key_to_direction(key_code);
    if (direction != -1)
    {
        return direction;
    }

    return key_code;
}

void print_output(const char *format, ...)
{
    char buffer[256];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    printw("%s", buffer);
}

void print_output_at(int row, int col, const char *format, ...)
{
    char buffer[256];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    mvprintw(row, col, "%s", buffer);
}

void platform_sleep(int millis)
{
    struct timespec sleep_time;
    
    if (millis < 1000)
    {
        sleep_time.tv_sec = 0;
        sleep_time.tv_nsec = millis * 1000000;
    }
    else
    {
        sleep_time.tv_sec = millis / 1000;
        sleep_time.tv_nsec = 0;
    }
    
    nanosleep(&sleep_time, NULL);
}