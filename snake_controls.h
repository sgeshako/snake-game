#include "constants.h"

typedef struct {
	e_direction next_states[4];     
} direction_config_t;

/**
 * @brief Initialize Snake controls.
 */
void snake_controls_init(int* p_snake, int* p_snake_length, int (*p_snake_map_coordinates)[SIZE]);

/**
 * Give the next direction or keep the current if next is not allowed.
 *
 * @param input The requested new direction.
 * 
 * @return The next direction if allowed.
 */
e_direction new_direction(e_direction input);

/**
 * Move Snake exactly one position forward.
 *
 * @param p_head Pointer to Snake head coordinates.
 * @param change_direction New direction for Head to point to (optional). 
 * 
 */
void move_snake(point_t * p_head, e_direction change_direction);