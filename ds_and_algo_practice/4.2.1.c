#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

Node *head = NULL;

void insertNode(int data)
{
    Node *newNode = malloc(sizeof(Node));
    if (!newNode)
    {
        perror("メモリ割り当てエラー");
        exit(EXIT_FAILURE);
    }

    newNode->data = data;
    newNode->next = head;
    head = newNode;
}

void deleteNode(int data)
{
    Node *current = head, *previous = NULL;

    while (current && current->data != data)
    {
        previous = current;
        current = current->next;
    }

    if (!current)
    {
        printf("削除対象のデータが見つかりませんでした: %d\n", data);
        return;
    }

    if (!previous)
    {
        head = current->next;
    }
    else
    {
        previous->next = current->next;
    }

    free(current);
    printf("データ %d を削除しました\n", data);
}

void searchNode(int data)
{
    Node *current = head;

    while (current && current->data != data)
    {
        current = current->next;
    }

    if (!current)
    {
        printf("データ %d は見つかりませんでした\n", data);
    }
    else
    {
        printf("データ %d が見つかりました\n", data);
    }
}

void printList(void)
{
    Node *current = head;

    printf("リストの内容: ");
    while (current)
    {
        printf("[%d] ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main()
{
    insertNode(10);
    insertNode(20);
    insertNode(30);

    printList();

    searchNode(20);

    searchNode(40);

    deleteNode(20);

    printList();

    return 0;
}
