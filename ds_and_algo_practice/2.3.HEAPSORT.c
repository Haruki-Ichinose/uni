#include <stdio.h>
#include <limits.h>

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
        return -1;
    }
    int max = arr[0];
    arr[0] = arr[heapsize - 1];
    heapsize = heapsize - 1;
    int i;
    MAXHEAPIFY(arr, 0, heapsize);
    return max;
}

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

void HEAPSORT2(int arr[], int heapsize)
{
    BUILDMAXHEAP2(arr, heapsize);
    for (int i = heapsize - 1; i >= 1; i--)
    {
        arr[i] = HEAPEXTRACTMAX(arr, i + 1);
    }
}

int main()
{
    int arr[] = {61, 91, 33, 54, 58, 62, 50, 58, 27, 1, 11, 16};
    int heapsize = sizeof(arr) / sizeof(arr[0]);

    HEAPSORT2(arr, heapsize);

    for (int i = heapsize - 1; 0 <= i; i--)
    {
        printf("%d ", arr[i]);
    }
    return 0;
}