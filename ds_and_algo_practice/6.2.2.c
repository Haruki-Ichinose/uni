#include <stdio.h>
#include <stdlib.h>

#define MAX 100
#define INF 1000000

typedef struct
{
    int queue[MAX];
    int head;
    int tail;
} Queue;

void init(Queue *q)
{
    q->head = 0;
    q->tail = 0;
}

void enqueue(Queue *q, int x)
{
    q->queue[q->tail++] = x;
}

int dequeue(Queue *q)
{
    return q->queue[q->head++];
}

int isEmpty(Queue *q)
{
    return q->head == q->tail;
}

int bfs(int graph[MAX][MAX], int start, int goal, int n)
{
    Queue q;
    init(&q);
    int visited[MAX];
    for (int i = 0; i < n; i++)
    {
        visited[i] = 0;
    }
    enqueue(&q, start);
    visited[start] = 1;
    while (!isEmpty(&q))
    {
        int node = dequeue(&q);
        if (node == goal)
        {
            return 1;
        }
        for (int i = 0; i < n; i++)
        {
            if (graph[node][i] && !visited[i])
            {
                enqueue(&q, i);
                visited[i] = 1;
            }
        }
    }
    return 0;
}

int main()
{
    int graph[MAX][MAX] = {
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
        {0, 0, 0, 0}};
    int start = 0;
    int goal = 3;
    int n = 4;
    if (bfs(graph, start, goal, n))
    {
        printf("There is a path from %d to %d\n", start, goal);
    }
    else
    {
        printf("There is no path from %d to %d\n", start, goal);
    }
    return 0;
}