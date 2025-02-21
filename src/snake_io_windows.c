#include "snake_io.h"
#include <stdio.h>
#include <windows.h>
#include "constants.h"
#include "utils.h"

/**
 * Maximum timeout to wait for user input in milliseconds.
 */
#define INPUT_TIMEOUT_OVERRIDE_MS 50

static int input_timeout_ms = -1;

static HANDLE hConsole = NULL;
static HANDLE hInput = NULL;

void snake_io_init(int timeout_ms)
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    hInput = GetStdHandle(STD_INPUT_HANDLE);
    
    // Shorter input timeout for Windows works better
    input_timeout_ms = INPUT_TIMEOUT_OVERRIDE_MS;
}

void snake_io_exit()
{

}

int wait_for_input()
{
    int key = NO_INPUT;
    
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    double elapsed_ms = 0;

    // Try to get a key press blocking for 50ms
    if (WaitForSingleObject(hInput, input_timeout_ms) == WAIT_OBJECT_0)
    {
        DWORD events;
        INPUT_RECORD inputRecord;
        
        ReadConsoleInput(hInput, &inputRecord, 1, &events);
        key = inputRecord.Event.KeyEvent.wVirtualKeyCode;

        QueryPerformanceCounter(&end);
        elapsed_ms = (end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
    }
    
    // if early, wait for the remaining ms 
    while (elapsed_ms < input_timeout_ms)
    {   
        // sleep in 25ms chunks
        Sleep(25);
        QueryPerformanceCounter(&end);
        elapsed_ms = (end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
    }

    return key;
}

static e_direction map_key_to_direction(int key_code)
{
    static const int key_map[] = {
        37,  // 0: LEFT  
        39,  // 1: RIGHT 
        38,  // 3: UP   
        40   // 4: DOWN  
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
    COORD pos = { 10, 10 };

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    DWORD written;
    WriteConsoleOutputCharacter(hConsole, buffer, strlen(buffer), pos, &written);
}

void print_output_at(int row, int col, const char *format, ...)
{
    char buffer[256];
    COORD pos = { col, row };

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    DWORD written;
    WriteConsoleOutputCharacter(hConsole, buffer, strlen(buffer), pos, &written);
}

void refresh_screen()
{

}

void clear_screen()
{
    system("cls");
}

void flush_input_buffer()
{
    FlushConsoleInputBuffer(hInput);
}

void platform_sleep(int millis)
{
    Sleep(millis);
}
