#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include <ncurses.h>
#include <unistd.h>
#include <time.h>
// #include <limits.h>
// #include "snake_io.h"
#include "snake_gui.h"
#include "snake_controls.h"
#include "constants.h"

#define CENTER (SIZE / 2)

#define INPUT_TIMEOUT_MS 100

static const int initial_snake[]  = { RIGHT, RIGHT, RIGHT, RIGHT };
// static const int initial_snake[]  = { RIGHT, RIGHT, RIGHT, RIGHT, UP, UP, RIGHT, RIGHT, RIGHT, UP, UP, RIGHT };
static const int initial_snake_size = sizeof(initial_snake) / sizeof(initial_snake[0]);

static int snake[TOTAL];
static int snake_length;

static int snake_map_coordinates[SIZE][SIZE] = {0};

static point_t head = { .x = CENTER, .y = CENTER };
static point_t tail = {};

static point_t food = { .x = 14, .y = 7 };

static void init_snake()
{
    memcpy(&snake, initial_snake, initial_snake_size * sizeof(int));
    snake_length = initial_snake_size;

    snake_controls_init(snake, &snake_length, snake_map_coordinates);
    snake_gui_init(snake_map_coordinates);
}

int monitor_snake()
{
    if (head.x == food.x && head.y == food.y)
    {
        shift_right(snake, snake_length);
        snake_length = snake_length + 1;
     
        place_food(&food);
    }
    
    if (snake_map_coordinates[head.y][head.x])
    {
        mvprintw(CENTER, CENTER, "YOU LOST...\n");
        refresh();

        struct timespec sleep_time;
        sleep_time.tv_sec = 3;
        sleep_time.tv_nsec = 0;
        nanosleep(&sleep_time, NULL);
        
        return 0;
    }
    
    return 1;
}

int main(void)
{
    int key;

    // Initialize ncurses
    initscr();              // Start curses mode
    // cbreak();               // Disable line buffering
    // noecho();               // Don't echo input
    keypad(stdscr, TRUE);   // Enable special keys (e.g., arrow keys)
    timeout(INPUT_TIMEOUT_MS);           // Wait for input for 100ms, then return ERR
    // nodelay(stdscr, TRUE);  // Non-blocking mode

    printw("Press arrow keys (Left, Right, Up, Down) to move. Press 'q' to quit.\n");
    refresh();

    // Initialize snake array.
    init_snake();

    // Initial draw of snake on plot
    // init_draw_snake(snake, snake_length, tail, &head);

    draw_snake(snake, snake_length, head);
    mvprintw(1, 0, "Head calculated at: (%d, %d)", head.y, head.x);
    mvprintw(0, 0, "AT moment before move: (%d, %d)", tail.y, tail.x);
    printw("\n");
    refresh();

    int snake_alive = 1;

    while (snake_alive) {
        mvprintw(0, 0, "Food: (%d, %d)", food.y, food.x);
        mvprintw(1, 0, "Head: (%d, %d)", head.y, head.x);
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        long elapsed_ms = 0;

        // Try to get a key press blocking for 100ms
        key = getch();
        while (elapsed_ms < INPUT_TIMEOUT_MS)
        {
            struct timespec sleep_time;
            sleep_time.tv_sec = 0;
            sleep_time.tv_nsec = 25 * 1000000;
            nanosleep(&sleep_time, NULL);

            clock_gettime(CLOCK_MONOTONIC, &end);
            elapsed_ms = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
        }
        
        // for (size_t i = 0; i < snake_length; i++)
        // {
        //     mvprintw(3, i + 2, "%c ", snake[i] ? (snake[i] == 1 ? 'R' : (snake[i] == 2 ? 'U' : 'D')) : 'L');
        // }
        // refresh();
        
        
        if (key != ERR)
        {
            switch (key) 
            {
                case KEY_DOWN:
                case KEY_UP:
                case KEY_RIGHT:
                case KEY_LEFT:
                    move_snake(&head, new_direction(key));
                    flushinp(); // flush input buffer
                    break;       
                case 'q':
                    printw("Exiting...\n");
                    refresh();
                    endwin(); // End curses mode
                    return 0;
                default:
                    printw("Unknown key: %d\n", key);
                    break;
            }
        }
        else {
            move_snake(&head, -1);
        }

        snake_alive = monitor_snake();
        clear();
        
        draw_snake(snake, snake_length, head);
        draw_food(food);
        draw_border();

        refresh(); // Refresh the screen to show the output
    }

    endwin(); // End curses mode
    return 0;
    
    
}