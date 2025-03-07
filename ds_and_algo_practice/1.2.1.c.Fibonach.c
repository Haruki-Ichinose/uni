#include <stdio.h>

double Fib_tail(int n, double a, double b)
{
    if (n == 0)
    {
        return b;
    }
    if (n == 1)
    {
        return a + b;
    }
    return Fib_tail(n - 1, b, a + b);
}

double Fib(int n)
{
    return Fib_tail(n, 1, 0);
}

int main(void)
{
    int n;
    scanf("%d", &n);
    printf("%f", Fib(n));
}