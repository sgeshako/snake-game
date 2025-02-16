#include "constants.h"

// void snake_gui_init(int* p_snake, int* p_snake_length);
void snake_gui_init(int (*p_snake_map_coordinates)[24]);

void draw_snake(int snake[24 * 24], int snake_length, point_t head);

void draw_food(point_t food);

void place_food(point_t* p_food);