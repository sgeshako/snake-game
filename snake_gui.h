#include "constants.h"

/**
 * @brief Initialize Snake GUI.
 */
void snake_gui_init(int (*p_snake_map_coordinates)[SIZE]);

/**
 * Draw Snake on game plot beginning from head position.
 *
 * @param snake The array containing actual Snake.
 * @param snake_length The length of actual Snake.
 * @param head The current head position.
 * 
 */
void draw_snake(int snake[TOTAL], int snake_length, point_t head);

/**
 * Draw food on the game plot.
 *
 * @param food The new food position.
 * 
 */
void draw_food(point_t food);

/**
 * Place food on game plot randomly.
 *
 * @param p_food Pointer to actual food position.
 * 
 */
void place_food(point_t* p_food);

/**
 * @brief Draw border around game plot.
 */
void draw_border();