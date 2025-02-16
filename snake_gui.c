#include "snake_gui.h"
// #include <stddef.h>
#include "snake_controls.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

// static int (*mp_snake)[1000] = NULL;
// static int *mp_snake_length = NULL;
static int (*mp_snake_map_coordinates)[24] = NULL;

// void snake_gui_init(int* p_snake, int* p_snake_length)
void snake_gui_init(int (*p_snake_map_coordinates)[24])
{
    mp_snake_map_coordinates = p_snake_map_coordinates;
    // mp_snake = p_snake;
    // mp_snake_length = p_snake_length;
}

void draw_snake(int snake[24 * 24], int snake_length, point_t head)
{
    int curr_x = head.x;
    int curr_y = head.y;
    // clear snake coordinates
    // memset(mp_snake_map_coordinates, 0, 24 * 24 * sizeof(int));

    for (int i = snake_length - 1; i >= 0; i--)
    {
        int temp_curr_x = curr_x;
        int temp_curr_y = curr_y;
        char d;
        
        switch (snake[i])
        {
            case LEFT:
                curr_x++;
                d = 'L';
                break;
            case RIGHT:
                curr_x--;
                d = 'R';
                break;
            case UP:
                curr_y++;
                d = 'U';
                break;
            case DOWN:
                curr_y--;
                d = 'D';
                break;
            default:
                return;
        }

        mvprintw(temp_curr_y, temp_curr_x, "%c", d);
        // mp_snake_map_coordinates[temp_curr_y][temp_curr_x] = 1;
    }
}

void place_food(point_t* p_food)
{
    for (size_t i = 0; i < 5; i++)
    {
        int rand_x = rand() % (24 + 1);
        int rand_y = rand() % (24 + 1);
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
    mvprintw(food.y, food.x, "*");
}