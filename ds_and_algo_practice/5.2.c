#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M 10
#define NIL -1
#define DELETED -2
#define MAX_USERNAME 50
#define MAX_PASSWORD 50

typedef struct
{
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
} Account;

int hash(char *str, int i)
{
    int h = 0;
    while (*str)
    {
        h = (h * 256 + *str) % M;
        str++;
    }
    return (h + i) % M;
}

void HashInsert(Account T[], char *username, char *password)
{
    int i = 0;
    int j;
    do
    {
        j = hash(username, i);
        if (T[j].username[0] == NIL || T[j].username[0] == DELETED)
        {
            strcpy(T[j].username, username);
            strcpy(T[j].password, password);
            return;
        }
        else
        {
            i++;
        }
    } while (i != M);
    printf("Hash table overflow\n");
}

int HashSearch(Account T[], char *username)
{
    int i = 0;
    int j;
    do
    {
        j = hash(username, i);
        if (strcmp(T[j].username, username) == 0)
        {
            return j;
        }
        else
        {
            i++;
        }
    } while (T[j].username[0] != NIL && i != M);
    return NIL;
}

int HashDelete(Account T[], char *username)
{
    int i = 0;
    int j;
    do
    {
        j = hash(username, i);
        if (strcmp(T[j].username, username) == 0)
        {
            T[j].username[0] = DELETED;
            return j;
        }
        else
        {
            i++;
        }
    } while (T[j].username[0] != NIL && i != M);
    return NIL;
}

int main()
{
    Account T[M];
    for (int i = 0; i < M; i++)
    {
        T[i].username[0] = NIL;
    }

    // アカウント作成
    HashInsert(T, "user1", "password123");
    HashInsert(T, "user2", "mypassword");

    int pos = HashSearch(T, "user1");
    if (pos != NIL)
    {
        printf("User1 found at position %d\n", pos);
    }
    else
    {
        printf("User1 not found\n");
    }
    HashDelete(T, "user1");
    pos = HashSearch(T, "user1");
    if (pos != NIL)
    {
        printf("User1 found at position %d\n", pos);
    }
    else
    {
        printf("User1 not found\n");
    }

    return 0;
}
