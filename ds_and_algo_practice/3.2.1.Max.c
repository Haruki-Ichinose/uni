#include <stdio.h>

int Maximum(int arr[], int n)
{
    int max = arr[0];
    for (int i = 1; i < n; i++)
    {
        if (max < arr[i])
        {
            max = arr[i];
        }
    }
    return max;
}

int main()
{
    int arr[] = {3, 7, 1, 9, 5, 4};
    int n = sizeof(arr) / sizeof(arr[0]);
    int max = Maximum(arr, n);

    printf("Maximum value: %d\n", max);

    return 0;
}