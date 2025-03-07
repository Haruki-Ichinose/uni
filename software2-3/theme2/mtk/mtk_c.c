#include <stdio.h>
#include "mtk_c.h"

/*======================================================
変数および配列の定義
========================================================*/
SEMAPHORE_TYPE semaphore[NUMSEMAPHORE]; /* セマフォ */
TCB_TYPE task_tab[NUMTASK + 1];         /* TCB配列 */
STACK_TYPE stacks[NUMTASK];             /* タスクスタック */

/* 大域変数 */
TASK_ID_TYPE curr_task;
TASK_ID_TYPE new_task;
TASK_ID_TYPE next_task;
TASK_ID_TYPE ready;

/*======================================================
カーネルの初期化
init_kernel:
引数 : なし
返り値 : なし
担当 : 若松
========================================================*/

void init_kernel()
{
  /* TCB配列の初期化. TCB配列の各要素はTCB_TYPE型の構造体 */
  for (int i = 0; i <= NUMTASK; i++)
  {                                /* id = 0 は使わないが初期化は行っておく */
    task_tab[i].status = EMPTY;    /* status = EMPTY */
    task_tab[i].next = NULLTASKID; /* 次のタスクidはNULLTASKID */
    task_tab[i].task_addr = NULL;  /* タスクアドレス = NULL */
    task_tab[i].stack_ptr = NULL;  /* スタックアドレス = NULL*/
    task_tab[i].priority = 0;      /* 優先度は0 */
  }

  /* readyキューの初期化 */
  ready = 0; /* 実行待ちタスクはない */

  /* pr_handler = TRAP #1 */
  *(unsigned int *)0x084 = (unsigned int)pv_handler; /* 関数名でアドレス参照 */

  /* セマフォの値を初期化 */
  for (int i = 0; i < NUMSEMAPHORE; i++)
  {
    semaphore[i].count = 1;              /* セマフォは専有されていない */
    semaphore[i].task_list = NULLTASKID; /* 初期化時点で次のタスクはない */
  }
}

/*======================================================
ユーザタスクの初期化と登録
set_task:
引数 : ユーザタスク関数へのポインタ(タスクの先頭番地)
返り値 : なし
担当：一瀬
========================================================*/

void set_task(void *p)
{
  TASK_ID_TYPE i;
  for (int i = 1; i <= NUMTASK; i++)
  {
    if (task_tab[i].status == EMPTY)
    {
      new_task = i;
      break;
    } // タスクを走査し、空きスロット見つけたらそのIDをnew_taskに代入
  }
  task_tab[new_task].task_addr = p;                    // 空きスロットにユーザタスク関数のポインタを代入
  task_tab[new_task].status = OCCUPIED;                // スロットの使用状態を使用中に変更
  task_tab[new_task].stack_ptr = init_stack(new_task); // init_stackで初期化
  addq(&ready, new_task);                              // readyキューにnew_taskを登録
}

/*==========================================================================
ユーザタスク用のスタックの初期化
init_stack:
引数   : タスクID
返り値 : 初期化後にユーザタスク用SSPが指すアドレス(void *型)
担当：若松、一瀬（共同）
===========================================================================*/

void *init_stack(TASK_ID_TYPE id)
{
  int *int_ssp;
  int_ssp = (int *)&stacks[id - 1].sstack[STKSIZE]; // Sスタックの底+1のアドレスをポインタに代入

  *(--int_ssp) = (int)task_tab[id].task_addr; // 初期PCを積む

  unsigned short int *short_ssp = (unsigned short int *)int_ssp;
  *(--short_ssp) = 0x0000; // 初期SRを積む
  int_ssp = (int *)short_ssp;

  int_ssp -= 15;                                  // 15×4バイト分の領域を飛ばす（レジスタ群を積む）
  *(--int_ssp) = &stacks[id - 1].ustack[STKSIZE]; // 初期USPを積む

  return (void *)int_ssp; // 関数終了時のSSPが返り値
}

/*==========================================================================
マルチタスク処理の開始
begin_sch:
引数 : なし
返り値 : なし
担当 : 若松
===========================================================================*/

void begin_sch()
{
  curr_task = removeq(&ready); /* removeqで最初のタスクを設定 */
  init_timer();                /* init_timerでタイマの初期化 */
  first_task();                /* first_taskで最初のタスクを起動 */
}

/*=============================================================
タスクキューの最後尾へのTCBの追加
addq:
引数 : キューへのポインタとタスクのID
返り値 : なし
担当：一瀬
===============================================================*/

void addq(TASK_ID_TYPE *que_ptr, TASK_ID_TYPE id)
{
  if (*que_ptr == NULLTASKID)
  {                // キューの先頭のタスクが空なら,
    *que_ptr = id; // 先頭にタスクを登録
  }
  else
  {
    TCB_TYPE *task_ptr = &task_tab[*que_ptr]; // 先頭のタスクのポインタ設定
    while (1)
    {
      if ((*task_ptr).next == NULLTASKID)
      {                        // その次のタスクが空だったら,
        (*task_ptr).next = id; // タスクを登録
        break;
      }
      else
      {
        task_ptr = &task_tab[(*task_ptr).next]; // 次のタスクにポインタを移動
      }
    }
  }
}

/*=============================================================
タスクキューの先頭からTCBの除去
removeq:
引数 : キューへのポインタ
返り値 : 先頭のID
担当：一瀬
===============================================================*/
TASK_ID_TYPE removeq(TASK_ID_TYPE *que_ptr)
{
  TASK_ID_TYPE r_id = *que_ptr; // 返却値（先頭のタスクのidを取得）
  if (r_id != NULLTASKID)
  {                                       // キューの先頭が空でなければ
    TCB_TYPE *task_ptr = &task_tab[r_id]; // 先頭のタスクのポインタ設定
    *que_ptr = (*task_ptr).next;          // 先頭から2番目のタスクを先頭にする
    (*task_ptr).next = NULLTASKID;        // 先頭のタスクのnextはNULLTASKIDにして、タスクを取り出す
  }
  return r_id; // キューの先頭のタスクのidを返す
}

/*=============================================================
タスクのスケジュール関数
sched:
引数 : なし
返り値 : なし
担当 : 若松
===============================================================*/

void sched()
{
  next_task = removeq(&ready); /* readyキューからタスクを取り出しnext_taskに代入 */
  while (next_task == NULLTASKID)
  {
    while (1)
      ;
  } /* NULLTASKIDの場合は無限ループ */
}

void p_body(int id)
{
  semaphore[id].count--; /*セマフォの値をデクリメント*/
  if (semaphore[id].count < 0)
  {
    /*その結果セマフォが獲得できないなら*/
    sleep(id); /*セマフォ待ちのキューに入れ休眠状態へ*/
  }
}

void v_body(int id)
{
  semaphore[id].count++; /*セマフォの値をインクリメント*/
  if (semaphore[id].count <= 0)
  {             /*その結果セマフォが空いたなら*/
    wakeup(id); /*そのセマフォを待っている先頭のタスクを実行可能状態へ*/
  }
}

int sleep(int a)
{ /* a := セマフォID*/
  addq(&(semaphore[a].task_list), curr_task);
  sched();
  swtch();
}

int wakeup(int a)
{
  TASK_ID_TYPE task_id = removeq(&(semaphore[a].task_list));
  addq(&ready, task_id);
}
