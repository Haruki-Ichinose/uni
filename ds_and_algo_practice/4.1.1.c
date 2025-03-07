#include <stdio.h>

#define STACK_SIZE 10

int stack[STACK_SIZE];
int head_index = -1;

void push(int value)
{
    if (head_index < STACK_SIZE - 1)
    {
        head_index++;
        stack[head_index] = value;
        printf("Pushed %d onto the stack.\n", value);
    }
    else
    {
        printf("Stack overflow. Cannot push %d\n", value);
    }
}

int pop()
{
    if (head_index >= 0)
    {
        int value = stack[head_index];
        head_index--;
        printf("Popped %d from the stack.\n", value);
        return value;
    }
    else
    {
        printf("Stack underflow. Cannot pop from an empty stack.\n");
        return -1;
    }
}

int main()
{
    push(5);
    push(10);
    push(15);
    pop();
    pop();

    return 0;
}