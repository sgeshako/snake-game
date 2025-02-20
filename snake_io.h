#ifdef __linux__
    #include <ncurses.h>
#endif

#define NO_INPUT -1

#ifdef __linux__
    #define refresh_screen() refresh()

    #define clear_screen() clear()

    #define flush_input_buffer() flushinp()
#endif
#ifdef _WIN32
    void refresh_screen();

    void clear_screen();

    void flush_input_buffer();
#endif

void snake_io_init(int timeout_ms);

void snake_io_exit();

int wait_for_input();

int normalize_input(int key_code);

void print_output(const char *format, ...);

void print_output_at(int row, int col, const char *format, ...);

void platform_sleep(int millis);
