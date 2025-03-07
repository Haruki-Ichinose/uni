#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

typedef struct Stack
{
    Node *top;
} Stack;

typedef struct Queue
{
    Node *front;
    Node *rear;
} Queue;

// スタックの操作

// スタックを初期化する関数
void initializeStack(Stack *stack)
{
    stack->top = NULL;
}

// スタックに要素を追加する関数（Push）
void push(Stack *stack, int data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode)
    {
        perror("メモリ割り当てエラー");
        exit(EXIT_FAILURE);
    }

    newNode->data = data;
    newNode->next = stack->top;
    stack->top = newNode;
}

// スタックから要素を取り出す関数（Pop）
int pop(Stack *stack)
{
    if (stack->top == NULL)
    {
        fprintf(stderr, "スタックが空です\n");
        exit(EXIT_FAILURE);
    }

    int data = stack->top->data;
    Node *temp = stack->top;
    stack->top = stack->top->next;
    free(temp);

    return data;
}

// キューの操作

// キューを初期化する関数
void initializeQueue(Queue *queue)
{
    queue->front = NULL;
    queue->rear = NULL;
}

// キューに要素を追加する関数（Enqueue）
void enqueue(Queue *queue, int data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode)
    {
        perror("メモリ割り当てエラー");
        exit(EXIT_FAILURE);
    }

    newNode->data = data;
    newNode->next = NULL;

    if (queue->rear == NULL)
    {
        // キューが空の場合
        queue->front = newNode;
        queue->rear = newNode;
    }
    else
    {
        // キューが空でない場合
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// キューから要素を取り出す関数（Dequeue）
int dequeue(Queue *queue)
{
    if (queue->front == NULL)
    {
        fprintf(stderr, "キューが空です\n");
        exit(EXIT_FAILURE);
    }

    int data = queue->front->data;
    Node *temp = queue->front;

    if (queue->front == queue->rear)
    {
        // キューに1つの要素しかない場合
        queue->front = NULL;
        queue->rear = NULL;
    }
    else
    {
        // キューに複数の要素がある場合
        queue->front = queue->front->next;
    }

    free(temp);
    return data;
}

int main()
{
    Stack myStack;
    initializeStack(&myStack);

    push(&myStack, 10);
    push(&myStack, 20);
    push(&myStack, 30);

    printf("Stack Pop: %d\n", pop(&myStack));
    printf("Stack Pop: %d\n", pop(&myStack));

    Queue myQueue;
    initializeQueue(&myQueue);

    enqueue(&myQueue, 40);
    enqueue(&myQueue, 50);
    enqueue(&myQueue, 60);

    printf("Queue Dequeue: %d\n", dequeue(&myQueue));
    printf("Queue Dequeue: %d\n", dequeue(&myQueue));

    return 0;
}
