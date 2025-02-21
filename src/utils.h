/**
 * Return the first index of a given integer that is present in the array.
 *
 * @param arr The integer array.
 * @param size Size of the array.
 * @param target Integer 
 * 
 * @return The index of the found integer or -1.
 */
int index_of(const int arr[], int size, int target);

/**
 * Shift the given number of array elements to the left.
 * Last element remains untouched.
 *
 * @param arr The array to be shifted.
 * @param size The number of elements to shift.
 * 
 */
void shift_left(int arr[], int size); 

/**
 * Shift the given number of array elements to the right.
 * First element remains untouched.
 *
 * @param arr The array to be shifted.
 * @param size The number of elements to shift.
 * 
 */
void shift_right(int arr[], int size);