#ifndef MTK_C_H
#define MTK_C_H

// カーネル関連の定数定義///////////////////////////////////////////
#define NUMSEMAPHORE 5
#define NULLTASKID 0
#define NUMTASK 5
#define STKSIZE 1024

// task_tab.status 定数////////////////////////////////////////////
#define EMPTY 0
#define OCCUPIED 1
#define COMPLETED 2

// 構造体の定義//////////////////////////////////////////////////

typedef int TASK_ID_TYPE; /* TASK_ID_TYPEの定義 */

typedef struct
{
  int count;
  int nst; /* reserved */
  TASK_ID_TYPE task_list;
} SEMAPHORE_TYPE;

typedef struct
{
  void (*task_addr)();
  void *stack_ptr;
  int priority;
  int status;
  TASK_ID_TYPE next;
} TCB_TYPE;

typedef struct
{
  char ustack[STKSIZE];
  char sstack[STKSIZE];
} STACK_TYPE;

// 大域変数、配列の定義///////////////////////////////////////////

extern TASK_ID_TYPE curr_task;
extern TASK_ID_TYPE new_task;
extern TASK_ID_TYPE next_task;
extern TASK_ID_TYPE ready;

extern SEMAPHORE_TYPE semaphore[NUMSEMAPHORE];
extern TCB_TYPE task_tab[NUMTASK + 1];
extern STACK_TYPE stacks[NUMTASK];

// 関数のプロトタイプ宣言///////////////////////////////////////////
extern void outbyte(int ch, unsigned char c);
extern char inbyte(int ch);

/// マルチタスクカーネル関連の関数

extern void init_kernel();
extern void set_task(void *p);
extern void *init_stack(TASK_ID_TYPE id);
extern void begin_sch();
extern void addq(TASK_ID_TYPE *que_ptr, TASK_ID_TYPE id);
extern TASK_ID_TYPE removeq(TASK_ID_TYPE *que_ptr);

extern void sched();

extern void first_task();
extern void swtch();

/// タイマ関連の関数
extern void hard_clock();
extern void init_timer();
///// モニタ内の関数
extern void set_timer();
extern void reset_timer();

/// セマフォ関連の関数
extern void p_body(int a);
extern void v_body(int a);

extern int sleep(int a);
extern int wakeup(int a);

extern void P(int id);
extern void V(int id);
extern void pv_handler();

extern void skipmt();

#endif