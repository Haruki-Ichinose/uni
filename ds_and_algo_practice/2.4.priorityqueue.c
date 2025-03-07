#include <stdio.h>
#include <float.h>

int heapsize = 0;

void MAXHEAPIFY(float arr[], int i)
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
        float tmp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = tmp;
        MAXHEAPIFY(arr, largest);
    }
}

int HEAPEXTRACTMAX(float arr[])
{
    if (heapsize < 1)
    {
        printf("Heap underflow\n");
        return -1;
    }
    float max = arr[0];
    arr[0] = arr[heapsize - 1];
    heapsize--;
    MAXHEAPIFY(arr, 0);
    return max;
}

void HEAPINCREASEKEY(float arr[], int i, float key)
{
    if (key < arr[i])
    {
        printf("新しいキーは現在のキー以下です。\n");
        return;
    }
    arr[i] = key;
    while (i > 0 && arr[(i - 1) / 2] < arr[i])
    {
        float tmp = arr[i];
        arr[i] = arr[(i - 1) / 2];
        arr[(i - 1) / 2] = tmp;
        i = (i - 1) / 2;
    }
}

void MAXINSERTHEAP(float arr[], float key)
{
    if (heapsize == FLT_MAX)
    {
        printf("Heap overflow\n");
        return;
    }
    heapsize++;
    arr[heapsize - 1] = -FLT_MIN;
    HEAPINCREASEKEY(arr, heapsize - 1, key);
}

int main()
{
    const int LENGTH = 10;
    float heap[LENGTH];

    MAXINSERTHEAP(heap, 20);
    MAXINSERTHEAP(heap, 15);
    MAXINSERTHEAP(heap, 30);
    MAXINSERTHEAP(heap, 10);

    printf("最大値: %f\n", HEAPEXTRACTMAX(heap));

    printf("次の最大値: %f\n", HEAPEXTRACTMAX(heap));

    printf("キューの残りの要素:\n");
    for (int i = 0; i < heapsize; i++)
    {
        printf("%f ", heap[i]);
    }
    printf("\n");

    return 0;
}
