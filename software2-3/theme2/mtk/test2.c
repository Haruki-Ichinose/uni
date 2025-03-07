#include <stdio.h>
#include "mtk_c.h" // マルチタスクカーネル用ヘッダー
int N = 3;
int K = 10000;
volatile int nttask;

int task_0()
{
	while (1)
	{
		printf(" task0 is runnnig\n");
		P(0);
		if (nttask == N)
		{
			nttask = 0;
			for (int i = 0; i < N; i++)
			{
				printf("  do V(1) ,%d times\n", i + 1);
				V(1);
			}
			printf("tasks reset\n");
		}
		V(0);
		printf("jmp to skipmt\n");
		skipmt();
		printf("task0 is back\n");
	}
}

int task1()
{
	while (1)
	{
		printf(" task1 start\n");
		for (int k = 0; k < K; k++)
		{
			if (k % 1000 == 0)
			{
				printf("task1 ,%d times\n", k / 1000);
			}
		}
		printf(" task1 is runnnig\n");
		P(0);
		nttask++;
		printf("nttask is %d now\n", nttask);
		V(0);
		printf("              task1 is finished\n");
		P(1);
	}
}

int task2()
{
	while (1)
	{
		printf(" task2 start\n");
		for (int k = 0; k < 2 * K; k++)
		{
			if (k % 1000 == 0)
			{
				printf("task2 ,%d times\n", k / 1000);
			}
		}
		printf(" task2 is runnnig\n");
		P(0);
		nttask++;
		printf("nttask is %d now\n", nttask);
		V(0);
		printf("              task2 is finished\n");
		P(1);
	}
}

int task3()
{
	while (1)
	{
		printf(" task3 start\n");
		for (int k = 0; k < 3 * K; k++)
		{
			if (k % 1000 == 0)
			{
				printf("task3 ,%d times\n", k / 1000);
			}
		}
		printf(" task3 is runnnig\n");
		P(0);
		nttask++;
		printf("nttask is %d now\n", nttask);
		V(0);
		printf("             task3 is finished\n");
		P(1);
	}
}

int main()
{

	printf("Initializing kernel...\n");
	init_kernel();
	printf("Kernel initialized.\n");

	semaphore[0].count = 1;
	semaphore[1].count = 0;
	nttask = 0;

	printf("Setting Task 0...\n");
	set_task((char *)task_0); // タスクを登録
	printf("Task 0 set.\n");

	printf("Setting Task 1...\n");
	set_task((char *)task1); // タスクを登録
	printf("Task 1 set.\n");

	printf("Setting Task 2...\n");
	set_task((char *)task2); // タスクを登録
	printf("Task 2 set.\n");

	printf("Setting Task 3...\n");
	set_task((char *)task3); // タスクを登録
	printf("Task 3 set.\n");

	printf("Starting scheduler...\n");
	begin_sch();

	// begin_sch後のコードは実行されないはず
	printf("Scheduler started (this line should not be printed).\n");

	return 0;
}

/* exit() defined in test*.c */
void exit(int value)
{
	*(char *)0x00d00039 = 'h'; /* led0 への表示 (halt) */
	for (;;)
		; /* 無限ループトラップで停止させる */
}
