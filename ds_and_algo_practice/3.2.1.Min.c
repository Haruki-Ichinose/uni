#include <stdio.h>

int Minimum(int arr[], int n)
{
    int min = arr[0];
    for (int i = 1; i < n; i++)
    {
        if (min > arr[i])
        {
            min = arr[i];
        }
    }
    return min;
}

int main()
{
    int arr[] = {3, 7, 1, 9, 5, 4};
    int n = sizeof(arr) / sizeof(arr[0]);
    int min = Minimum(arr, n);

    printf("Minimum value: %d\n", min);

    return 0;
}