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

void BUILDMAXHEAP(int arr[], int heapsize)
{
    int length = heapsize;
    for (int i = length / 2 - 1; i >= 0; i--)
    {
        MAXHEAPIFY(arr, i, heapsize);
    }
}

int HEAPEXTRACTMAX(int arr[], int heapsize)
{
    if (heapsize < 1)
    {
        printf("Heap underflow\n");
        return -1;
    }
    int max = arr[0];
    arr[0] = arr[heapsize - 1];
    MAXHEAPIFY(arr, 0, heapsize - 1);
    return max;
}

void HEAPSORT(int arr[], int heapsize)
{
    BUILDMAXHEAP(arr, heapsize);
    for (int i = heapsize - 1; i >= 1; i--)
    {
        arr[i] = HEAPEXTRACTMAX(arr, i + 1);
    }
}

int main()
{
    int arr[] = {61, 91, 33, 54, 58, 62, 50, 58, 27, 1, 11, 16};
    int heapsize = sizeof(arr) / sizeof(arr[0]);

    HEAPSORT(arr, heapsize);

    for (int i = 0; i < heapsize; i++)
    {
        printf("%d ", arr[i]); // 昇順で出力
    }
    return 0;
}