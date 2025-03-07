#include <stdio.h>
#include <stdlib.h>

#define M 10
#define NIL -1
#define DELETED -2

int hash(int k, int i)
{
    return (k + i) % M;
}

int hash_quadratic(int k, int i)
{
    int c1 = 1, c2 = 3;
    return ((k % M) + c1 * i + c2 * i * i) % M;
}

// ダブルハッシュ法用の第二ハッシュ関数
int second_hash(int k)
{
    return (1 + (k % (M - 1)));
}

// ダブルハッシュ法用のハッシュ関数
int hash_double(int k, int i)
{
    return (k % M + i * second_hash(k)) % M;
}

void HashInsert(int T[], int k)
{
    int i = 0;
    int j;
    do
    {
        j = hash(k, i);
        if (T[j] == NIL || T[j] == DELETED)
        {
            T[j] = k;
            return;
        }
        else
        {
            i++;
        }
    } while (i != M);
    printf("Hash table overflow\n");
}

int HashSearch(int T[], int k)
{
    int i = 0;
    int j;
    do
    {
        j = hash(k, i);
        if (T[j] == k)
        {
            return j;
        }
        else
        {
            i++;
        }
    } while (T[j] != NIL && i != M);
    return NIL;
}

void HashDelete(int T[], int k)
{
    int i = HashSearch(T, k);
    if (i != NIL)
    {
        T[i] = DELETED;
    }
}

int main()
{
    int T[M];
    for (int i = 0; i < M; i++)
    {
        T[i] = NIL;
    }

    // テスト用のコード
    HashInsert(T, 5);
    HashInsert(T, 15);
    HashInsert(T, 25);
    printf("25の位置: %d\n", HashSearch(T, 25));
    HashDelete(T, 15);
    printf("15の位置: %d\n", HashSearch(T, 15));

    return 0;
}
