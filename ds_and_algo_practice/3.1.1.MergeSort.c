#include <stdio.h>
#include <limits.h>

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

int main()
{
    int arr[] = {12, 11, 13, 5, 6, 7, 1, 3, 2, 4, 9, 8, 10, 20, 19, 18, 17, 16, 15, 14};
    /*上の{}内にソートしたい配列を入力してください*/
    int arr_size = sizeof(arr) / sizeof(arr[0]);

    MergeSort(arr, 0, arr_size - 1);

    for (int i = 0; i < arr_size; i++)
    {
        printf("%d ", arr[i]);
    }

    return 0;
}