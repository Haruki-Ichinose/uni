#include <stdio.h>

#define QUEUE_SIZE 10

int queue[QUEUE_SIZE];
int front = -1;
int rear = -1;

void enqueue(int value)
{
    if ((rear + 1) % QUEUE_SIZE == front)
    {
        printf("Queue overflow. Cannot enqueue %d\n", value);
    }
    else
    {
        if (front == -1)
        {
            front = 0;
        }
        rear = (rear + 1) % QUEUE_SIZE;
        queue[rear] = value;
        printf("Enqueued %d into the queue.\n", value);
    }
}

int dequeue()
{
    int value;
    if (front == -1)
    {
        printf("Queue underflow. Cannot dequeue from an empty queue.\n");
        return -1;
    }
    else
    {
        value = queue[front];
        if (front == rear)
        {
            front = rear = -1;
        }
        else
        {
            front = (front + 1) % QUEUE_SIZE;
        }
        printf("Dequeued %d from the queue.\n", value);
        return value;
    }
}

int main()
{
    enqueue(5);
    enqueue(10);
    enqueue(15);

    int dequeued_value = dequeue();

    return 0;
}
