#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "snake_io.h"
#include "snake_gui.h"
#include "snake_controls.h"
#include "constants.h"

/* Center location of game plot. */
#define CENTER (SIZE / 2)

/**
 * Maximum timeout to wait for user input in milliseconds.
 */
#define INPUT_TIMEOUT_MS 100

/* Values used to initialize Snake. */
static const int initial_snake[]  = { RIGHT, RIGHT, RIGHT, RIGHT };
static const int initial_snake_size = sizeof(initial_snake) / sizeof(initial_snake[0]);

/**
 * Static array contains the actual Snake.
 */
static int snake[TOTAL];
static int snake_length; /**< The length of the Snake inside the static array. */

/**
 * Indicates current coordinates of Snake on the game plot.
 */
static int snake_map_coordinates[SIZE][SIZE] = {0};

/** Snake head coordinates. */
static point_t head = { .x = CENTER, .y = CENTER };
static point_t tail = {};

/** Food coordinates. */
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
        print_output_at(CENTER, CENTER, "YOU LOST...\n");
        refresh_screen();

        platform_sleep(3000);
        
        return 0;
    }
    
    return 1;
}

int main(void)
{
    int key;

    // Initialize I/O
    snake_io_init(INPUT_TIMEOUT_MS);

    // Initialize snake array.
    init_snake();

    // Initial draw of snake on plot
    draw_snake(snake, snake_length, head);
    refresh_screen();

    int snake_alive = 1;

    while (snake_alive) {
        print_output_at(0, 0, "Press arrow keys (Left, Right, Up, Down) to move. Press 'q' to quit.");
        print_output_at(1, 0, "Snake length: %d", snake_length);
        print_output_at(2, 0, "Food: (%d, %d)", food.y, food.x);
        print_output_at(3, 0, "Head: (%d, %d)", head.y, head.x);

        key = wait_for_input();
        
        if (key != NO_INPUT)
        {
            int normalized_input = normalize_input(key);
            switch (normalized_input) 
            {
                case DOWN:
                case UP:
                case RIGHT:
                case LEFT:
                    move_snake(&head, new_direction(normalized_input));
                    flush_input_buffer();
                    break;       
                case 'q':
                case 'Q':
                    print_output("Exiting...\n");
                    refresh_screen();
                    snake_io_exit();
                    return 0;
                default:
                    print_output("Unknown key: %d\n", key);
                    break;
            }
        }
        else {
            move_snake(&head, -1);
        }

        snake_alive = monitor_snake();
        clear_screen();
        
        draw_snake(snake, snake_length, head);
        draw_food(food);
        draw_border();

        refresh_screen(); // Refresh the screen to show the output
    }

    snake_io_exit();
    return 0;
    
    
}