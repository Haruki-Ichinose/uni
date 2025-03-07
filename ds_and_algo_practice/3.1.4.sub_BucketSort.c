#include <stdio.h>

void insertionSort(int arr[], int n)
{
    int i, j, key;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void Bucketsort(int arr[], int n)
{
    int B[n];
    for (int i = 0; i <= n; i++)
    {
        /*B[i]を空リストに初期化する*/
    }
    for (int i = 1; i = n;)
    {
        /*A[i]をリストB[nA[i]]に挿入する*/
    }
    for (int i = 0; i <= n - 1; i++)
    {
        /*リストB[i]を挿入ソートでソートする*/
    }
    /*リストB[0],B[1],…,B[n-1]を連接する*/
}