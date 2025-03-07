#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

void Merge(int arr[], int p, int q, int r)
{
    int n1 = q - p + 1;
    int n2 = r - q;
    int L[n1 + 1], R[n2 + 1];

    for (int i = 0; i < n1; i++)
    {
        L[i] = arr[p + i];
    }
    for (int j = 0; j < n2; j++)
    {
        R[j] = arr[q + j + 1];
    }

    L[n1] = INT_MAX;
    R[n2] = INT_MAX;

    int i = 0, j = 0;
    for (int k = p; k <= r; k++)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
    }
}

void MergeSort(int arr[], int p, int r)
{
    if (p < r)
    {
        int q = (p + r) / 2;
        MergeSort(arr, p, q);
        MergeSort(arr, q + 1, r);
        Merge(arr, p, q, r);
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

    MergeSort(array, 0, n - 1);

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