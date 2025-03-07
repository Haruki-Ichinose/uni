#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

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

int *CreateRandArray(int *n)
{
    printf("要素数を入力してください: ");
    if (scanf("%d", n) != 1 || *n <= 0)
    {
        printf("無効な入力です\n");
        return NULL;
    }

    int *array = (int *)malloc((*n) * sizeof(int));

    if (array == NULL)
    {
        printf("メモリの確保に失敗しました\n");
        return NULL;
    }

    srand(time(NULL)); // 乱数の種を設定

    for (int i = 0; i < *n; i++)
    {
        array[i] = rand() % 200000;
    }

    return array;
}

int main()
{
    int n;
    int *array = CreateRandArray(&n);

    if (array == NULL)
    {
        return 1; // エラーが発生した場合
    }

    struct timeval start_time, end_time;

    // 時間計測開始
    gettimeofday(&start_time, NULL);

    HEAPSORT(array, n);

    // 時間計測終了
    gettimeofday(&end_time, NULL);

    printf("ソートされた配列: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", array[i]);
    }

    // 処理にかかった時間を表示
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                          (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

    printf("\n処理にかかった時間: %f秒\n", elapsed_time);

    free(array);

    return 0;
}