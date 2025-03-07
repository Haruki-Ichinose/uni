
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int Partition(int arr[], int p, int r)
{
    int x = arr[r];
    int i = p - 1;
    int j;
    for (j = p; j < r; j++)
    {
        if (arr[j] <= x)
        {
            i++;
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
    }
    int tmp = arr[i + 1];
    arr[i + 1] = arr[j];
    arr[j] = tmp;
    return i + 1;
}

int RondomizedPartition(int arr[], int p, int r)
{
    int i = p + (int)(rand() * (r - p + 1) / (1 + RAND_MAX));
    int tmp = arr[r];
    arr[r] = arr[i];
    arr[i] = tmp;
    return Partition(arr, p, r);
}

int RandomizedSelect(int arr[], int p, int r, int i)
{
    if (p == r)
    {
        return arr[p];
    }
    int q = RondomizedPartition(arr, p, r);
    int k = q - p + 1;
    if (i == k)
    {
        return arr[q];
    }
    else if (i < k)
    {
        return RandomizedSelect(arr, p, q - 1, i);
    }
    else
    {
        return RandomizedSelect(arr, q + 1, r, i - k);
    }
}

int main()
{
    int arr[] = {4, 2, 7, 1, 9, 5, 3, 8, 6, 10, 11};
    int n = sizeof(arr) / sizeof(arr[0]);

    int i = n - (int)ceil((double)n / 3) + 1;

    int median = RandomizedSelect(arr, 0, n - 1, i);

    printf("ceil(n/3)番目の値: %d
", median);

    return 0;
}
