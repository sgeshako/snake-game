
// Function to find the index of a given integer in an array
int index_of(const int arr[], int size, int target) {
    for (int i = 0; i < size; i++) 
    {
        if (arr[i] == target) 
        {
            return i; // Return the index if the target is found
        }
    }
    return -1; // Return -1 if the target is not found
}