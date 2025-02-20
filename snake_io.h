#ifdef __linux__
    #include <ncurses.h>
#endif

/** Missing user input. */
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

/**
 * Initialize I/O implementation.
 *
 * @param timeout_ms The maximum timeout for user input in milliseconds.
 * 
 */
void snake_io_init(int timeout_ms);

/**
 * Cleanup of I/O.
 */
void snake_io_exit();

/**
 * Wait for user input until timeout expires.
 *
 * @return The pressed key code or NO_INPUT.
 */
int wait_for_input();

/**
 * Parse direction from key code when applicable.
 *
 * @param key_code The pressed key code.
 * 
 * @return The parsed direction or original key code.
 */
int normalize_input(int key_code);

/**
 * Print string to output at the current cursor position.
 */
void print_output(const char *format, ...);

/**
 * Print string to output at a given cursor position.
 *
 * @param row The row to move the cursor to.
 * @param col The col to move the cursor to.
 * 
 */
void print_output_at(int row, int col, const char *format, ...);

/**
 * Go to sleep for given period.
 *
 * @param millis The number of milliseconds to sleep for.
 * 
 */
void platform_sleep(int millis);
