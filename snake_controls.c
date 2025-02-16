#include "snake_controls.h"
#include <stddef.h>
#include "utils.h"
#include <ncurses.h>

static int *mp_snake = NULL;
static int *mp_snake_length = NULL;

void snake_controls_init(int* p_snake, int* p_snake_length)
{
    mp_snake = p_snake;
    mp_snake_length = p_snake_length;
}

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

void move_snake(point_t * p_head)
{
    int head_is = mp_snake[*mp_snake_length - 1];

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

    shift_left(mp_snake, *mp_snake_length);
}

void change_direction(int key, point_t * p_head)
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
        e_direction current_direction = mp_snake[*mp_snake_length - 1];
        e_direction next_direction = state_machine[current_direction].next_states[input];

        shift_left(mp_snake, *mp_snake_length);
        mp_snake[*mp_snake_length - 1] = next_direction;
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