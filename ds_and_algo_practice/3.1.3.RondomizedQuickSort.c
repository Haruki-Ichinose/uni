#include <stdio.h>
#include <stdlib.h>

int Partition(int arr[], int p, int r)
{
    int x = arr[r];
    int i = p - 1;
    int j;
    for (j = p; j < r; j++) /*なぜr-1ではなくrとするべきなのか*/
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
    /*ランダムに採択する*/
    int i = p + (int)(rand() * (r - p + 1) / (1 + RAND_MAX));
    int tmp = arr[r];
    arr[r] = arr[i];
    arr[i] = tmp;
    return Partition(arr, p, r);
}

void RondomizedQuickSort(int arr[], int p, int r)
{
    if (p < r)
    {
        int q = RondomizedPartition(arr, p, r); /*qは関数内で初期化するべきなのはなぜなのか。*/
        RondomizedQuickSort(arr, p, q - 1);
        RondomizedQuickSort(arr, q + 1, r);
    }
}

int main()
{
    int arr[] = {5, 2, 9, 1, 19, 10, 23, 5, 6};
    int n = sizeof(arr) / sizeof(arr[0]);
    RondomizedQuickSort(arr, 0, n - 1);

    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    return 0;
}