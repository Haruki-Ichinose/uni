#include <stdio.h>

void CountingSort(int arr[], int n)
{
    int max = arr[0];
    for (int i = 1; i < n; i++)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
    }

    int C[max + 1], B[n];

    // C の初期化
    for (int i = 0; i <= max; i++)
    {
        C[i] = 0;
    }

    // カウント
    for (int j = 0; j < n; j++)
    {
        C[arr[j]] = C[arr[j]] + 1;
    }

    // C を累積和に変更
    for (int i = 1; i <= max; i++)
    {
        C[i] = C[i] + C[i - 1];
    }

    // B にソートされた結果を格納
    for (int j = n - 1; j >= 0; j--)
    {
        B[C[arr[j]] - 1] = arr[j];
        C[arr[j]] = C[arr[j]] - 1;
    }

    // ソート結果を元の配列にコピー
    for (int i = 0; i < n; i++)
    {
        arr[i] = B[i];
    }
}

int main()
{
    int arr[] = {4, 2, 1, 6, 5, 3};
    int n = sizeof(arr) / sizeof(arr[0]);

    CountingSort(arr, n);

    printf("Sorted array: \n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    return 0;
}
