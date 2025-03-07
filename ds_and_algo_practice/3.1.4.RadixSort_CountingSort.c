#include <stdio.h>

void CountingSort(int arr[], int n, int dig)
{
    const int base = 10;
    int B[n];
    int C[base];

    for (int i = 0; i < base; i++)
    {
        C[i] = 0;
    }

    // カウント
    for (int i = 0; i < n; i++)
    {
        C[(arr[i] / dig) % base]++;
    }

    // C を累積和に変更
    for (int i = 1; i <= base; i++)
    {
        C[i] = C[i] + C[i - 1];
    }

    // B にソートされた結果を格納
    for (int j = n - 1; j >= 0; j--)
    {
        B[C[arr[j] / dig % base] - 1] = arr[j];
        C[arr[j] / dig % base]--;
    }

    // ソート結果を元の配列にコピー
    for (int i = 0; i < n; i++)
    {
        arr[i] = B[i];
    }
}

void RadixSort(int arr[], int n, int d)
{
    int max = arr[0];
    for (int i = 1; i < n; i++)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
    }

    for (int dig = 1; max / dig > 0; dig *= 10)
    {
        CountingSort(arr, n, dig);
    }
}

int main()
{
    int arr[] = {170, 45, 75, 90, 802, 24, 2, 66, 1000, 1020, 30120, 4059};
    int n = sizeof(arr) / sizeof(arr[0]);

    RadixSort(arr, n, 5);

    printf("Sorted array: \n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    return 0;
}