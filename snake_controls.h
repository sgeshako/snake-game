#include "constants.h"

#ifndef DIRECTIONS_H
#define DIRECTIONS_H

typedef enum
{
    LEFT,
    RIGHT,
    UP,
    DOWN
} e_direction;

#endif

typedef struct {
	e_direction next_states[4];     
} direction_config_t;

void snake_controls_init(int* p_snake, int* p_snake_length, int (*p_snake_map_coordinates)[SIZE]);

e_direction new_direction(int key);

void move_snake(point_t * p_head, e_direction change_direction);

void change_direction(int key, point_t * p_head);