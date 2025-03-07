#include <stdio.h>

double Fib(int n)
{
    if (n == 0)
    {
        return 0;
    }
    if (n == 1)
    {
        return 1;
    }
    if (n == 2)
    {
        return 1;
    }
    return Fib(n - 1) + Fib(n - 2);
}

int main(void)
{
    int n;
    scanf("%d", &n);
    printf("%f", Fib(n));
}