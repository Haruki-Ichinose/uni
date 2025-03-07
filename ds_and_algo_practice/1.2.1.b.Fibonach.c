#include <stdio.h>
double Fib(int n)
{
    if (n <= 1)
    {
        return n;
    }
    double tmp, prv, cur;
    prv = 0;
    cur = 1;
    for (int i = 2; i <= n; i++)
    {
        tmp = cur;
        cur = cur + prv;
        prv = tmp;
    }
    return cur;
}

int main(void)
{
    int n;
    scanf("%d", &n);
    printf("%f", Fib(n));
}