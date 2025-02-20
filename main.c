#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include <unistd.h>
#include <time.h>
#include "snake_io.h"
#include "snake_gui.h"
#include "snake_controls.h"
#include "constants.h"

#define CENTER (SIZE / 2)

#define INPUT_TIMEOUT_MS 100

static const int initial_snake[]  = { RIGHT, RIGHT, RIGHT, RIGHT };
// static const int initial_snake[]  = { RIGHT, RIGHT, RIGHT, RIGHT, UP, UP, RIGHT, RIGHT, RIGHT, UP, UP, RIGHT };
static const int initial_snake_size = sizeof(initial_snake) / sizeof(initial_snake[0]);

static int snake[TOTAL];
static int snake_length;

static int snake_map_coordinates[SIZE][SIZE] = {0};

static point_t head = { .x = CENTER, .y = CENTER };
static point_t tail = {};

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

        // struct timespec sleep_time;
        // sleep_time.tv_sec = 3;
        // sleep_time.tv_nsec = 0;
        // nanosleep(&sleep_time, NULL);
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

    print_output("Press arrow keys (Left, Right, Up, Down) to move. Press 'q' to quit.\n");
    refresh_screen();

    // Initialize snake array.
    init_snake();

    // Initial draw of snake on plot
    draw_snake(snake, snake_length, head);
    refresh_screen();

    int snake_alive = 1;

    while (snake_alive) {
        print_output_at(0, 0, "Snake length: %d", snake_length);
        print_output_at(1, 0, "Food: (%d, %d)", food.y, food.x);
        print_output_at(2, 0, "Head: (%d, %d)", head.y, head.x);

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