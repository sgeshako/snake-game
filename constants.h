#ifndef CONSTANTS_H
#define CONSTANTS_H

typedef struct {
    int x;
    int y;
} point_t;

#define SIZE 24
#define TOTAL (SIZE * SIZE)

typedef enum
{
    LEFT,
    RIGHT,
    UP,
    DOWN
} e_direction;

#endif // CONSTANTS_H