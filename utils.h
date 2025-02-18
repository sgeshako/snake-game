#ifndef DIRECTIONS_H
#define DIRECTIONS_H

typedef enum
{
    LEFT,
    RIGHT,
    UP,
    DOWN
} e_direction;

#endif

// Function to find the index of a given integer in an array.
int index_of(const int arr[], int size, int target);

// Fuction to shift array one element to the left.
void shift_left(int arr[], int size); 

void shift_right(int arr[], int size);