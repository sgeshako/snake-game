#ifndef CONSTANTS_H
#define CONSTANTS_H

typedef struct {
    int x;
    int y;
} point_t;

/**
 * Size of the game plot for rows and cols.
 */
#define SIZE 24

/**
 * Total size of the game plot (rows x cols).
 */
#define TOTAL (SIZE * SIZE)

typedef enum
{
    LEFT,
    RIGHT,
    UP,
    DOWN
} e_direction;

#endif // CONSTANTS_H