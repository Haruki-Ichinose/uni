#include <stdio.h>

void Hanoi(int n, char str, char cen, char dst)
{
    if (n >= 2)
    {
        Hanoi(n - 1, str, dst, cen);
    }
    printf("%d枚目を %c から %c へ\n", n, str, dst);

    if (n >= 2)
    {
        Hanoi(n - 1, cen, str, dst);
    }
}

int main(void)
{
    int n;
    char A = 'A';
    char B = 'B';
    char C = 'C';
    scanf("%d", &n);
    Hanoi(n, A, B, C);
    return 0;
}
