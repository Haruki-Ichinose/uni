#include <stdio.h>

void MAXHEAPIFY(int arr[], int i, int heapsize)
{
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    int largest;
    if (l < heapsize && arr[l] > arr[i])
    {
        largest = l;
    }
    else
    {
        largest = i;
    }
    if (r < heapsize && arr[r] > arr[largest])
    {
        largest = r;
    }
    if (largest != i)
    {
        int tmp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = tmp;
        MAXHEAPIFY(arr, largest, heapsize);
    }
}

int HEAPEXTRACTMAX(int arr[], int heapsize)
{
    if (heapsize < 1)
    {
        printf(" Heap underflow");
    }
    int max = arr[0];
    arr[0] = arr[heapsize - 1];
    heapsize = heapsize - 1;
    int i;
    MAXHEAPIFY(arr, 0, heapsize);
    return max;
}