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

#define INPUT_TIMEOUT_MS 200

typedef struct {
    int x;
    int y;
} point_t;

// static const int initial_snake[]  = { LEFT, LEFT, LEFT, LEFT, DOWN, DOWN, DOWN, DOWN, RIGHT, RIGHT, RIGHT, RIGHT, UP, UP, RIGHT };
static const int initial_snake[]  = { RIGHT, RIGHT, RIGHT, RIGHT, UP, UP, RIGHT, RIGHT, RIGHT, UP, UP, RIGHT };
static const int initial_snake_size = sizeof(initial_snake) / sizeof(initial_snake[0]);

static int snake[TOTAL];
static int snake_length;

static bool food = false;
static int curr_x = 5;
static int curr_y = 11;

void draw_snake_coordinates(int snake[TOTAL], int plot[SIZE][SIZE])
{
    int temp_curr_x = curr_x;
    int temp_curr_y = curr_y;

    for (size_t i = 0; i < TOTAL; i++)
    {
        switch (snake[i])
        {
            case LEFT:
                plot[temp_curr_y][++temp_curr_x] = 'L';
                break;
            case RIGHT:
                plot[temp_curr_y][--temp_curr_x] = 'R';
                break;
            case UP:
                plot[++temp_curr_y][temp_curr_x] = 'U';
                break;
            case DOWN:
                plot[--temp_curr_y][temp_curr_x] = 'D';
                break;
            default:
                return;
        }
    }
    
}

void plot_board(int plot[SIZE][SIZE])
{
    static int snake_coordinates[TOTAL][2] = {};
    
    draw_snake_coordinates(snake, plot);
    // for (size_t i = 0; i < 15; i++)
    // {
    //     printf("(%d, %d)\n", snake_coordinates[i][0], snake_coordinates[i][1]);
    // }
    

    // update boundaries
    for (size_t row = 0; row < SIZE; row++)
    {
        for (size_t col = 0; col < SIZE; col++)
        {
            if (row == 0 || row == SIZE - 1)
            {
                plot[row][col] = '@';
            }
            else if (col == 0 || col == SIZE - 1)
            {
                plot[row][col] = '#';
            }
        }
    }

    for (size_t row = 0; row < SIZE; row++)
    {
        for (size_t col = 0; col < SIZE; col++)
        {
            printf(" %c", plot[row][col]);
        }
        printf("\n");
    }
    
}

static void init_snake()
{
    memcpy(&snake, initial_snake, initial_snake_size * sizeof(int));
    snake_length = initial_snake_size;
}

static void init_draw_snake(int snake[TOTAL], int snake_length, point_t tail, point_t * p_head)
{
    int temp_curr_x = tail.x;
    int temp_curr_y = tail.y;

    for (size_t i = 0; i < snake_length; i++)
    {
        switch (snake[i])
        {
            case LEFT:
                mvprintw(temp_curr_y, --temp_curr_x, "L");
                break;
            case RIGHT:
                mvprintw(temp_curr_y, ++temp_curr_x, "R");
                break;
            case UP:
                mvprintw(--temp_curr_y, temp_curr_x, "U");
                break;
            case DOWN:
                mvprintw(++temp_curr_y, temp_curr_x, "D");
                break;
            default:
                return;
        }
    }

    p_head->x = temp_curr_x;
    p_head->y = temp_curr_y;
}

void shiftLeft(int arr[], int size) 
{
    // Shift all elements to the left
    for (int i = 0; i < size - 1; i++) {
        arr[i] = arr[i + 1];
    }

    // Set the last element to 0 (or any default value)
    // arr[size - 1] = 0;
}

static void moveSnake(point_t * p_tail, point_t * p_head)
{
    

    int tail_is = snake[0];
    static int i = 1;

    // mvprintw(i++, 0, "Current action: %s", tail_is ? (tail_is == 1 ? "RIGHT" : (tail_is == 2 ? "UP" : "DOWN")) : "LEFT");
    // printw("\n");

    switch (tail_is) {
        case DOWN:
            mvprintw(p_tail->y += 1, p_tail->x, " ");
            break;
        case UP:
            mvprintw(p_tail->y -= 1, p_tail->x, " ");
            break;
        case RIGHT:
            mvprintw(p_tail->y, p_tail->x += 1, " ");
            break;
        case LEFT:
            mvprintw(p_tail->y, p_tail->x -= 1,  " ");
            break;
    }

    // mvprintw(0, 0, "AT moment after move: (%d, %d)", p_tail->y, p_tail->x);
    // printw("\n");

    // refresh();
    int head_is = snake[snake_length - 1];

    switch (head_is) {
        case DOWN:
            mvprintw(p_head->y += 1,p_head->x,  "D");
            break;
        case UP:
            mvprintw( p_head->y -= 1,p_head->x, "U");
            break;
        case RIGHT:
            mvprintw(p_head->y,p_head->x += 1,  "R");
            break;
        case LEFT:
            mvprintw(p_head->y,p_head->x -= 1,  "L");
            break;
    }

    shiftLeft(snake, snake_length);
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

static void change_direction(int key)
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

        snake[snake_length - 1] = next_direction;
    }
    else
    {
        printw("Uknownn key: %d\n", key);
    }
    
    // switch (key) {
    //         case KEY_DOWN:
    //             snake[snake_length - 1] = DOWN;
    //             break;
    //         case KEY_UP:
    //             snake[snake_length - 1] = UP;
    //             break;
    //         case KEY_RIGHT:
    //             snake[snake_length - 1] = RIGHT;
    //             break;
    //         case KEY_LEFT:
    //             snake[snake_length - 1] = LEFT;
    //             break;
    //         case 'q':
    //             printw("Exiting...\n");
    //             refresh();
    //             endwin(); // End curses mode
    //             return 0;
    //         default:
    //             // printw("Unknown key: %d\n", key);
    //             break;
    //     }
}

int main(void)
{
    // int plot [SIZE][SIZE] = {};

    // for (size_t i = 0; i < SIZE; i++)
    // {
    //     for (size_t j = 0; j < SIZE; j++)
    //     {
    //         plot[i][j] = '.';
    //     }
    // }

    // plot_board(plot);


    


    // e_direction my_D = DOWN;
    // printf(" d: %d \n", my_D);
    // printf(" sum: %d\n", DOWN + 5);



    int key;

    // Initialize ncurses
    initscr();              // Start curses mode
    // cbreak();               // Disable line buffering
    // noecho();               // Don't echo input
    keypad(stdscr, TRUE);   // Enable special keys (e.g., arrow keys)
    // timeout(INPUT_TIMEOUT_MS);           // Wait for input for 100ms, then return ERR
    nodelay(stdscr, TRUE);  // Non-blocking mode

    printw("Press arrow keys (Left, Right, Up, Down) to move. Press 'q' to quit.\n");
    refresh();

    // point_t head = { .x = 15, .y = 7 };
    // point_t tail = { .x = 14, .y = 5 };

    init_snake();

    point_t head = {};
    point_t tail = { .x = CENTER, .y = CENTER };

    init_draw_snake(snake, snake_length, tail, &head);

    mvprintw(1, 0, "Head calculated at: (%d, %d)", head.y, head.x);
    // for (size_t i = 0; i < abs(head.y - tail.y); i++)
    // {
    //     mvprintw(tail.x, tail.y + i, "o");
    // }
    
    mvprintw(0, 0, "AT moment before move: (%d, %d)", tail.y, tail.x);
    printw("\n");
    refresh();

    while (1) {
        // for (size_t i = 0; i < snake_length; i++)
        // {
        //     printw(" %d", snake[i]);
        // }
        // sleep(1);

        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        long elapsed_ms = 0;
        
        bool handle_input = true;
        flushinp(); // flush input buffer

        while (elapsed_ms < INPUT_TIMEOUT_MS)
        {
            if (handle_input && (key = getch()) != ERR) 
            {
                // Handle input
                change_direction(key);
                handle_input = false;
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            elapsed_ms = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
        }
        
        
        // Try to get a key press blocking for 100ms
        
        // clear();
        moveSnake(&tail, &head);

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