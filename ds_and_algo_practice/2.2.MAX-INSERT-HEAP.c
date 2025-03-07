#include <stdio.h>
#include <limits.h>

void HEAPINCREASEKEY(int arr[], int i, int key)
{
    if (key < arr[i])
    {
        printf("新しいキーは現在のキー以下です。\n");
        return;
    }
    arr[i] = key;
    while (i > 0 && arr[(i - 1) / 2] < arr[i])
    {
        int tmp = arr[i];
        arr[i] = arr[(i - 1) / 2];
        arr[(i - 1) / 2] = tmp;
        i = (i - 1) / 2;
    }
}

void MAXINSERTHEAP(int arr[], int heapsize, int key)
{
    arr[heapsize] = INT_MIN;
    HEAPINCREASEKEY(arr, heapsize, key);
}

void BUILDMAXHEAP2(int arr[], int length)
{
    int heapsize = 0;
    for (int i = 0; i < length; i++)
    {
        heapsize++;
        MAXINSERTHEAP(arr, heapsize - 1, arr[i]);
    }
}