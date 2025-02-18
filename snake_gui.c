#include "snake_gui.h"
// #include <stddef.h>
#include "snake_controls.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include "constants.h"

#define OFFSET 5

#define DRAW_GAME_PLOT(y, x, ...) mvprintw((y) + OFFSET, (x) + OFFSET, __VA_ARGS__)


// static int (*mp_snake)[1000] = NULL;
// static int *mp_snake_length = NULL;
static int (*mp_snake_map_coordinates)[SIZE] = NULL;

// void snake_gui_init(int* p_snake, int* p_snake_length)
void snake_gui_init(int (*p_snake_map_coordinates)[SIZE])
{
    mp_snake_map_coordinates = p_snake_map_coordinates;
    // mp_snake = p_snake;
    // mp_snake_length = p_snake_length;
}

static inline int wrap_cursor(int next_value)
{
    // when going out of bounds wrap around
    if (next_value < 0 || next_value == SIZE)
    {
        return (next_value + SIZE) % SIZE;
    }
    
    return next_value;
}

void draw_snake(int snake[TOTAL], int snake_length, point_t head)
{
    int curr_x = head.x;
    int curr_y = head.y;

    for (int i = snake_length - 1; i >= 0; i--)
    {
        int temp_curr_x = curr_x;
        int temp_curr_y = curr_y;
        char d;
        
        switch (snake[i])
        {
            case LEFT:
                curr_x = wrap_cursor(curr_x + 1);
                d = 'L';
                break;
            case RIGHT:
                curr_x = wrap_cursor(curr_x - 1);
                d = 'R';
                break;
            case UP:
                curr_y = wrap_cursor(curr_y + 1);
                d = 'U';
                break;
            case DOWN:
                curr_y = wrap_cursor(curr_y - 1);
                d = 'D';
                break;
            default:
                return;
        }

        DRAW_GAME_PLOT(temp_curr_y, temp_curr_x, "%c", d);
    }
}

void place_food(point_t* p_food)
{
    for (size_t i = 0; i < 5; i++)
    {
        int rand_x = rand() % SIZE;
        int rand_y = rand() % SIZE;
        
        if (mp_snake_map_coordinates[rand_y][rand_x] == 0)
        {
            p_food->x = rand_x;
            p_food->y = rand_y;
            return;
        }
    }
    
    printf("ERROR: Couldn't place food randomly -.-\n");
}

void draw_food(point_t food)
{
    DRAW_GAME_PLOT(food.y, food.x, "*");
}

void draw_border()
{
    for (int i = -1; i < SIZE + 1; i++)
    {
        DRAW_GAME_PLOT(-1, i, "#");
        DRAW_GAME_PLOT(SIZE, i, "#");

        DRAW_GAME_PLOT(i, -1, "@");
        DRAW_GAME_PLOT(i, SIZE, "@");
    }
}