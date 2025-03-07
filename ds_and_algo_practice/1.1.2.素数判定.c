#include <stdio.h>
#include <math.h>

int isprime(int n)
{
    if (n < 2)
    {
        return 0;
    }
    if (n == 2)
    {
        return 1;
    }
    if (n % 2 == 0)
    {
        return 0;
    }
    int q = sqrt(n);
    for (int p = 3; p <= q; p += 2)
    {
        if (n % p == 0)
        {
            return 0;
        }
    }
    return 1;
}

int main()
{
    int n;
    scanf("%d", &n);
    if (isprime(n))
    {
        printf("%d is prime number.\n", n);
    }
    else
    {
        printf("%d is not prime number.\n", n);
    }
    return 0;
}