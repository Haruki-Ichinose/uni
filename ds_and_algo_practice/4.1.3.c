#include <stdio.h>

#define STACK_SIZE 5

int head_index = -1;

void push(int stack[], int value)
{
    if (head_index < STACK_SIZE - 1)
    {
        head_index++;
        stack[head_index] = value;
    }
    else
    {
        printf("Stack overflow. Cannot push %d\n", value);
    }
}

int pop(int stack[])
{
    if (head_index >= 0)
    {
        int value = stack[head_index];
        head_index--;
        return value;
    }
    else
    {
        printf("Stack underflow. Cannot pop from an empty stack.\n");
        return -1;
    }
}

void move(int src, int dest, int tower[][STACK_SIZE])
{
    int disk = pop(tower[src]);
    if (disk != -1)
    {
        push(tower[dest], disk);
    }
}

void Hanoi(int n, int src, int aux, int dest, int tower[][STACK_SIZE])
{
    if (n >= 1)
    {
        Hanoi(n - 1, src, dest, aux, tower);
        move(src, dest, tower);
        printf("%d枚目を %c から %c へ\n", n, 'A' + src, 'A' + dest);
        Hanoi(n - 1, aux, src, dest, tower);
    }
}

int main()
{
    int n;
    printf("円盤の枚数を入力してください: ");
    scanf("%d", &n);

    int tower[3][STACK_SIZE] = {0};

    for (int i = n; i > 0; i--)
    {
        push(tower[0], i);
    }

    Hanoi(n, 0, 1, 2, tower);

    return 0;
}
