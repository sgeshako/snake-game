#include "constants.h"

void snake_gui_init(int (*p_snake_map_coordinates)[SIZE]);

void draw_snake(int snake[TOTAL], int snake_length, point_t head);

void draw_food(point_t food);

void place_food(point_t* p_food);

void draw_border();