#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include <ncurses.h>
#include <unistd.h>
#include <time.h>
// #include <limits.h>

#define SIZE 24
#define TOTAL (SIZE * SIZE)

#define CENTER (SIZE / 2)

#define INPUT_TIMEOUT_MS 100

typedef struct {
    int x;
    int y;
} point_t;

static const int initial_snake[]  = { RIGHT, RIGHT, RIGHT, RIGHT };
// static const int initial_snake[]  = { RIGHT, RIGHT, RIGHT, RIGHT, UP, UP, RIGHT, RIGHT, RIGHT, UP, UP, RIGHT };
static const int initial_snake_size = sizeof(initial_snake) / sizeof(initial_snake[0]);

static int snake[TOTAL];
static int snake_length;

static bool food = false;

static void init_snake()
{
    memcpy(&snake, initial_snake, initial_snake_size * sizeof(int));
    snake_length = initial_snake_size;
}

static void draw_snake(int snake[TOTAL], int snake_length, point_t head)
{
    int temp_curr_x = head.x;
    int temp_curr_y = head.y;

    for (int i = snake_length - 1; i >= 0; i--)
    {
        switch (snake[i])
        {
            case LEFT:
                mvprintw(temp_curr_y, temp_curr_x++, "L");
                break;
            case RIGHT:
                mvprintw(temp_curr_y, temp_curr_x--, "R");
                break;
            case UP:
                mvprintw(temp_curr_y++, temp_curr_x, "U");
                break;
            case DOWN:
                mvprintw(temp_curr_y--, temp_curr_x, "D");
                break;
            default:
                return;
        }
    }
}

static void move_snake(point_t * p_head)
{
    int head_is = snake[snake_length - 1];

    switch (head_is) {
        case DOWN:
            p_head->y += 1;
            break;
        case UP:
            p_head->y -= 1;
            break;
        case RIGHT:
            p_head->x += 1;
            break;
        case LEFT:
            p_head->x -= 1;
            break;
    }

    shift_left(snake, snake_length);
}

static void place_food()
{
    if (!food)
    {
        mvprintw(8, 42, "*");
        food = true;
    }
}

typedef struct {
	e_direction next_states[4];     
} direction_config_t;

static e_direction get_direction(int key_code)
{
    static const int key_map[] = {
        KEY_LEFT,   // LEFT  
        KEY_RIGHT,  // RIGHT 
        KEY_UP,     // TOP   
        KEY_DOWN    // DOWN  
    };

    return index_of(key_map, sizeof(key_map) / sizeof(key_map[0]), key_code);    
}

static const direction_config_t state_machine[5] = {
		{ { LEFT, LEFT, UP, DOWN } },   // 0: LEFT
		{ { RIGHT, RIGHT, UP, DOWN } }, // 1: RIGHT
		{ { LEFT, RIGHT, UP, UP } },    // 2: UP
		{ { LEFT, RIGHT, DOWN, DOWN } } // 3: DOWN
};

static void change_direction(int key, point_t * p_head)
{
    switch (key)
    {
        case 'q':
            printw("Exiting...\n");
            refresh();
            endwin(); // End curses mode
            return;
        default:
            break;
    }

    e_direction input;
    if ((input = get_direction(key)) != -1)
    {
        e_direction current_direction = snake[snake_length - 1];
        e_direction next_direction = state_machine[current_direction].next_states[input];

        shift_left(snake, snake_length);
        snake[snake_length - 1] = next_direction;
        // snake_length = snake_length + 1;

        switch (next_direction) {
            case DOWN:
                p_head->y += 1;
                break;
            case UP:
                p_head->y -= 1;
                break;
            case RIGHT:
                p_head->x += 1;
                break;
            case LEFT:
                p_head->x -= 1;
                break;
        }
    }
    else
    {
        printw("Uknownn key: %d\n", key);
    }
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

    // point_t head = {};
    // point_t tail = { .x = CENTER, .y = CENTER };
    point_t head = { .x = CENTER, .y = CENTER };
    point_t tail = {};

    // Initial draw of snake on plot
    // init_draw_snake(snake, snake_length, tail, &head);

    draw_snake(snake, snake_length, head);
    mvprintw(1, 0, "Head calculated at: (%d, %d)", head.y, head.x);
    mvprintw(0, 0, "AT moment before move: (%d, %d)", tail.y, tail.x);
    printw("\n");
    refresh();

    while (1) {
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        long elapsed_ms = 0;
        
        bool handle_input = true;
        // flushinp(); // flush input buffer
        

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
            change_direction(key, &head);
            flushinp(); // flush input buffer
        }
        else {
            move_snake(&head);
        }
        clear();
        // moveSnake(&tail, &head);
        draw_snake(snake, snake_length, head);

        place_food();

        // switch (key) {
        //     case KEY_DOWN:
        //         mvprintw(tail.x++, head.y, " ");
        //         mvprintw(head.x += 1, head.y, "o");
        //         break;
        //     case KEY_UP:
        //         mvprintw(tail.x--, tail.y, " ");
        //         mvprintw(head.x -= 1, head.y, "o");
        //         break;
        //     case KEY_RIGHT:
        //         mvprintw(tail.x, tail.y++, " ");
        //         mvprintw(head.x, head.y += 1, "o");
        //         break;
        //     case KEY_LEFT:
        //         mvprintw(tail.x, tail.y--, " ");
        //         mvprintw(head.x, head.y -= 1, "o");
        //         break;
        //     case 'q':
        //         printw("Exiting...\n");
        //         refresh();
        //         endwin(); // End curses mode
        //         return 0;
        //     default:
        //         printw("Unknown key: %d\n", key);
        //         break;
        // }
        refresh(); // Refresh the screen to show the output
    }

    endwin(); // End curses mode
    return 0;
    
    
}