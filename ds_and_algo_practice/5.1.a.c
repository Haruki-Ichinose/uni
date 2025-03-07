#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int k;
    struct Node *next;
} Node;

typedef struct HashTable
{
    int size;
    Node **table;
} HashTable;

// ハッシュ関数
int hashFunction(HashTable *ht, int k)
{
    return k % ht->size;
}

// ハッシュテーブルの初期化
HashTable *createHashTable(int size)
{
    HashTable *ht = malloc(sizeof(HashTable));
    ht->size = size;
    ht->table = malloc(sizeof(Node *) * size);
    for (int i = 0; i < size; i++)
    {
        ht->table[i] = NULL;
    }
    return ht;
}

// ノードの挿入
void insertNode(HashTable *ht, int k)
{
    int index = hashFunction(ht, k);
    Node *newNode = malloc(sizeof(Node));
    if (!newNode)
    {
        perror("メモリ割り当てエラー");
        exit(EXIT_FAILURE);
    }
    newNode->k = k;
    newNode->next = ht->table[index];
    ht->table[index] = newNode;
}

// ノードの検索
Node *searchNode(HashTable *ht, int k)
{
    int index = hashFunction(ht, k);
    Node *current = ht->table[index];
    while (current)
    {
        if (current->k == k)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// ノードの削除
void deleteNode(HashTable *ht, int k)
{
    int index = hashFunction(ht, k);
    Node *current = ht->table[index], *previous = NULL;
    while (current)
    {
        if (current->k == k)
        {
            if (previous)
            {
                previous->next = current->next;
            }
            else
            {
                ht->table[index] = current->next;
            }
            free(current);
            printf("データ %d を削除しました\n", k);
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("削除対象のデータが見つかりませんでした: %d\n", k);
}

int main()
{
    HashTable *ht = createHashTable(10);

    insertNode(ht, 1);
    insertNode(ht, 2);
    insertNode(ht, 3);
    insertNode(ht, 11);

    Node *found = searchNode(ht, 11);
    if (found)
    {
        printf("データ %d が見つかりました\n", found->k);
    }
    else
    {
        printf("データが見つかりませんでした\n");
    }

    deleteNode(ht, 11);

    found = searchNode(ht, 11);
    if (found)
    {
        printf("データ %d が見つかりました\n", found->k);
    }
    else
    {
        printf("データが見つかりませんでした\n");
    }

    return 0;
}