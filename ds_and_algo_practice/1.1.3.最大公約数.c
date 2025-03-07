#include <stdio.h>

int gcd(int a, int b)
{
    int tmp;
    if (a < b)
    {
        tmp = a;
        a = b;
        b = tmp;
    }

    int r = a % b;
    while (r != 0)
    {
        a = b;
        b = r;
        r = a % b;
    }
    return b;
}

int main(void)
{
    int p, q;
    scanf("%d", &p);
    scanf("%d", &q);
    printf("%d", gcd(p, q));
    return 0;
}