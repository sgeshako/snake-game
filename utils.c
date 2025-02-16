
// Function to find the index of a given integer in an array.
int index_of(const int arr[], int size, int target) 
{
    for (int i = 0; i < size; i++) 
    {
        if (arr[i] == target) 
        {
            return i; // Return the index if the target is found
        }
    }
    return -1; // Return -1 if the target is not found
}

// Fuction to shift array one element to the left.
void shift_left(int arr[], int size) 
{
    // Shift all elements to the left
    for (int i = 0; i < size - 1; i++) {
        arr[i] = arr[i + 1];
    }

    // Last element remains unaltered
}

// Fuction to shift array one element to the right.
void shift_right(int arr[], int size) 
{
    // Shift all elements to the left
    for (int i = size - 1; i >= 0; i--) {
        arr[i + 1] = arr[i];
    }

    // First element remains unaltered
}