#include "snake_controls.h"
#include <stddef.h>
#include "utils.h"
#include <string.h>

static int *mp_snake = NULL;
static int *mp_snake_length = NULL;
static int (*mp_snake_map_coordinates)[SIZE] = NULL;

void snake_controls_init(int* p_snake, int* p_snake_length, int (*p_snake_map_coordinates)[SIZE])
{
    mp_snake = p_snake;
    mp_snake_length = p_snake_length;
    mp_snake_map_coordinates = p_snake_map_coordinates;
}

static const direction_config_t state_machine[] = {
		{ { LEFT, LEFT, UP, DOWN } },   // 0: LEFT
		{ { RIGHT, RIGHT, UP, DOWN } }, // 1: RIGHT
		{ { LEFT, RIGHT, UP, UP } },    // 2: UP
		{ { LEFT, RIGHT, DOWN, DOWN } } // 3: DOWN
};

e_direction new_direction(e_direction input)
{
    e_direction current_direction = mp_snake[*mp_snake_length - 1];
    e_direction next_direction = state_machine[current_direction].next_states[input];

    return next_direction;
}

static inline int wrap_around(int next_value)
{
    // when going out of bounds wrap around
    if (next_value < 0 || next_value == SIZE)
    {
        return (next_value + SIZE) % SIZE;
    }
    
    return next_value;
}

static void recalculate_snake_coordinates(point_t new_head)
{
    memset(mp_snake_map_coordinates, 0, SIZE * SIZE * sizeof(int));

    int curr_x = new_head.x;
    int curr_y = new_head.y;
    
    for (int i = *mp_snake_length - 1; i >= 0; i--)
    {
        /// fault
        switch (mp_snake[i])
        {
            case LEFT:
                curr_x = wrap_around(curr_x + 1);
                break;
            case RIGHT:
                curr_x = wrap_around(curr_x - 1);
                break;
            case UP:
                curr_y = wrap_around(curr_y + 1);
                break;
            case DOWN:
                curr_y = wrap_around(curr_y - 1);
                break;
            default:
                return;
        }
        mp_snake_map_coordinates[curr_y][curr_x] = 1;
    }
}

void move_snake(point_t * p_head, e_direction change_direction)
{
    shift_left(mp_snake, *mp_snake_length);

    if (change_direction != -1)
    {
        mp_snake[*mp_snake_length - 1] = change_direction;
    }
    
    e_direction head_is = mp_snake[*mp_snake_length - 1];

    switch (head_is) {
        case DOWN:
            p_head->y = wrap_around(p_head->y + 1);
            break;
        case UP:
            p_head->y = wrap_around(p_head->y - 1);
            break;
        case RIGHT:
            p_head->x = wrap_around(p_head->x + 1);
            break;
        case LEFT:
            p_head->x = wrap_around(p_head->x - 1);
            break;
    }

    recalculate_snake_coordinates(*p_head);
}
