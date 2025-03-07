#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

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

void QuickSort(int arr[], int p, int r)
{
    if (p < r)
    {
        int q = Partition(arr, p, r); /*qは関数内で初期化するべきなのはなぜなのか。*/
        QuickSort(arr, p, q - 1);
        QuickSort(arr, q + 1, r);
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

    QuickSort(array, 0, n - 1);

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