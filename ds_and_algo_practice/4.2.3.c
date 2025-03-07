#include <stdio.h>
#include <stdlib.h>

// Doubly linked listのノードを表す構造体
typedef struct Node
{
    int data;
    struct Node *prev;
    struct Node *next;
} Node;

// Doubly linked listを表す構造体
typedef struct DoublyLinkedList
{
    Node *head;
    Node *tail;
} DoublyLinkedList;

// Doubly linked listを初期化する関数
void initializeList(DoublyLinkedList *list)
{
    list->head = NULL;
    list->tail = NULL;
}

// ノードを挿入する関数
void insertNode(DoublyLinkedList *list, int data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode)
    {
        perror("メモリ割り当てエラー");
        exit(EXIT_FAILURE);
    }

    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = list->head;

    if (list->head == NULL)
    {
        // リストが空の場合
        list->tail = newNode;
    }
    else
    {
        // リストが空でない場合
        list->head->prev = newNode;
    }

    list->head = newNode;
}

// ノードを削除する関数
void deleteNode(DoublyLinkedList *list, int data)
{
    Node *current = list->head;

    while (current != NULL && current->data != data)
    {
        current = current->next;
    }

    if (current == NULL)
    {
        printf("削除対象のデータが見つかりませんでした: %d\n", data);
        return;
    }

    if (current->prev == NULL)
    {
        // 削除するノードが先頭の場合
        list->head = current->next;
        if (list->head != NULL)
        {
            list->head->prev = NULL;
        }
    }
    else
    {
        // 削除するノードが先頭でない場合
        current->prev->next = current->next;
        if (current->next != NULL)
        {
            current->next->prev = current->prev;
        }
        else
        {
            // 削除するノードが末尾の場合
            list->tail = current->prev;
        }
    }

    free(current);
    printf("データ %d を削除しました\n", data);
}

// ノードを検索する関数
void searchNode(const DoublyLinkedList *list, int data)
{
    Node *current = list->head;

    while (current != NULL && current->data != data)
    {
        current = current->next;
    }

    if (current == NULL)
    {
        printf("データ %d は見つかりませんでした\n", data);
    }
    else
    {
        printf("データ %d が見つかりました\n", data);
    }
}

// リストの内容を表示する関数
void displayList(const DoublyLinkedList *list)
{
    Node *current = list->head;

    printf("リストの内容（順方向）: ");
    while (current != NULL)
    {
        printf("[%d] ", current->data);
        current = current->next;
    }
    printf("\n");

    current = list->tail;
    printf("リストの内容（逆方向）: ");
    while (current != NULL)
    {
        printf("[%d] ", current->data);
        current = current->prev;
    }
    printf("\n");
}

// リストのメモリを解放する関数
void freeList(DoublyLinkedList *list)
{
    Node *current = list->head;
    Node *next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
}

int main()
{
    DoublyLinkedList myList;
    initializeList(&myList);

    // ノードを挿入
    insertNode(&myList, 10);
    insertNode(&myList, 20);
    insertNode(&myList, 30);

    // リストの内容を表示
    displayList(&myList);

    // ノードを検索
    searchNode(&myList, 20);

    // 存在しないデータを検索
    searchNode(&myList, 40);

    // ノードを削除
    deleteNode(&myList, 20);

    // 削除後のリストの内容を表示
    displayList(&myList);

    freeList(&myList);

    return 0;
}
