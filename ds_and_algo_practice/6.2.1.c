#include <stdio.h>

#define N 5

int visited[N + 1] = {0};

void DFS(int adjacent[N][N], int start, int target)
{
    visited[start] = 1;

    if (start == target)
    {
        printf("Found!\n");
        return;
    }

    for (int i = 0; i < N; i++)
    {
        if (adjacent[start][i] == 1 && visited[i] == 0)
        {
            DFS(adjacent, i, target);
        }
    }
}

int main()
{
    int adjacent[N][N] = {
        {0, 1, 0, 1, 0},
        {1, 0, 1, 1, 0},
        {0, 1, 0, 0, 1},
        {1, 1, 0, 0, 1},
        {0, 0, 1, 1, 0},
    };

    DFS(adjacent, 0, 4);

    return 0;
}