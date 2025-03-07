**global変数の宣言
.global	first_task
.global	swtch
.global hard_clock
.global init_timer
.global skipmt
.global	P
.global	V
.global	pv_handler

**外部入力（大域変数）
.extern curr_task
.extern next_task
.extern ready
.extern task_tab

**外部関数
.extern addq
.extern sched
.extern	p_body
.extern	v_body

**システムコール番号
.equ SYSCALL_NUM_GETSTRING,	1
.equ SYSCALL_NUM_PUTSTRING,	2
.equ SYSCALL_NUM_RESET_TIMER,   3
.equ SYSCALL_NUM_SET_TIMER,	4
.equ SYSCALL_NUM_SKIPMT, 5

.equ PV_CALL_P,			0
.equ PV_CALL_V,			1

.section .text

*********************************************************************************
** ユーザタスク起動用ルーチン
** first_task
** 入出力なし
** 担当：一瀬
**
*********************************************************************************
first_task:
    move.l	curr_task, %d0             /*curr_taskの番号をd1に*/	
    movea.l	#task_tab, %a0             /*task_tab配列の先頭アドレスをa0に*/

    mulu.w	#20, %d0                  /*curr_taskの番号に20を乗算し、d1に格納*/
    add.l   %d0, %a0                   /*a0にd1を加算し、curr_taskの先頭アドレスを計算*/
 
    movea.l %a0, %a1
    add.l   #4,  %a1                   /*a0は該当のcurr_taskのstack_ptr(SSP）の先頭アドレス*/

    move.l  (%a1), %sp                 /*スーパーバイザーモードのspにSSPを回復*/
    move.l	(%sp)+, %a2                
    move.l	%a2, %USP

    movem.l (%sp)+, %d0-%d7/%a0-%a6    /*レジスタ15本回復*/
        
    rte

***********************************************************************************
** タスクスイッチを実際に起こす関数
** swtch
** 入出力なし
** 担当：若松
***********************************************************************************
swtch:
	move.w	%SR,-(%sp)                 /* SRを退避しrteでの復帰を可能に */

	movem.l	%d0-%d7/%a0-%a6,-(%sp)	   /* 実行中タスクのレジスタ退避 */
	move.l	%usp, %a0
	move.l	%a0, -(%sp)
	
	move.l 	curr_task, %d1             /* curr_task -> d1 */
	mulu.w  #20, %d1                   /* TCB配列の各要素は4*5=20byte, タスクidに乗算 */
	movea.l #task_tab, %a0             /* task_tab配列の先頭アドレス -> a0 */
	add.l   %d1, %a0                   /* task_tab配列内のcurr_taskの先頭アドレスまで移動 */
	add.l   #4,  %a0                   /* stack_ptr(SSP）の先頭アドレス -> a0 */
	move.l  %sp, (%a0)                 /* 現在のタスクのTCBにSSPを記録*/
	
	move.l  next_task, %d1             /* next_task-> d1 */
	move.l  %d1, curr_task             /* curr_taskをnext_taskで更新*/
	mulu.w  #20, %d1                   /* TCB配列の各要素は4*5=20byte, タスクidに乗算 */
	movea.l #task_tab, %a0             /* task_tab配列の先頭アドレス -> a0*/
	add.l   %d1, %a0                   /* task_tab配列内のcurr_taskの先頭アドレスまで移動*/
	add.l   #4,  %a0                   /* stack_ptr(SSP）の先頭アドレス -> a0 */
 	move.l  (%a0),%sp                  /* スーパバイザモードのspにSSPを回復 */

 	move.l	(%sp)+, %a0
 	move.l	%a0, %USP   
	movem.l (%sp)+,%d0-%d7/%a0-%a6 	   /*次のタスクのレジスタを回復*/

	rte
        
        
hard_clock:
	movem.l	%d0-%d7/%a0-%a6, -(%sp)
	
	move.l	#ready, %d0			|%d0->readyキューへのポインタ
	move.l	curr_task, %d1			|%d1->タスクのID
	movem.l	%d0-%d1, -(%sp)		|%d0, %d1をスタックに積んで
	jsr	addq				|addqを実行
	adda.l	#8, %sp			|%d0, %d1の8バイト分を%spに加算
	
	jsr	sched
	jsr	swtch

	movem.l	(%sp)+, %d0-%d7/%a0-%a6
	
	rts     
        
        
init_timer:
	movem.l %d0-%d2, -(%sp) 
	move.l #SYSCALL_NUM_RESET_TIMER, %d0    | タイマーをリセット
	trap #0
    
	move.l #SYSCALL_NUM_SET_TIMER, %d0      | タイマーをセット
	move.w #1000, %d1			                                        | およそ１秒くらい
	move.l #hard_clock, %d2                                        | 割り込み時に呼び出すルーチンをセット
	trap #0
    
	movem.l (%sp)+,%d0-%d2
	rts  

skipmt:
	movem.l %d0-%d2, -(%sp) 
	move.l #SYSCALL_NUM_SKIPMT, %d0
	trap	#0
	
  	movem.l (%sp)+,%d0-%d2
	rts  

P:
	link.w	%a6,#0
	movem.l	%d0-%d1,-(%sp)  /*レジスタ退避*/
	
	move.l	#0,%d0          /*PシステムコールIDの0をd0レジスタにセット*/
	move.l	8(%a6),%d1      /*スタックから取り出した引数（セマフォID）をd1レジスタにセット*/
	trap	#1              
	
	movem.l	(%sp)+,%d0-%d1  /*レジスタ復帰*/
	unlk	%a6
	
	rts
	
V:
	link.w	%a6,#0
	movem.l	%d0-%d1,-(%sp)  /*レジスタ退避*/
	
	move.l	#1,%d0          /*VシステムコールIDの1をd1レジスタにセット*/
	move.l	8(%a6),%d1      /*スタックから取り出した引数（セマフォID)をd1レジスタにセット*/
	trap	#1
	
	movem.l	(%sp)+,%d0-%d1  /*レジスタ復帰*/
	unlk	%a6
	
	rts

pv_handler:
	movem.l %a0-%a6/%d0-%d7, -(%sp) 
	move.w	%SR,-(%sp)  /*SRをスタックに退避*/
	move.w	#0x2700,%SR /*走行レベルを７にして割り込み禁止*/
	
	movem.l	%d1,-(%sp)  /*レジスタd1をスタックに退避*/
	cmpi.l	#0,%d0      /*d0の値が0であるか比較*/
	bne	pv_handler_1    /*d0が0でないならば分岐*/
	jsr	p_body          /*p_body()を呼び出す*/
	bra	pv_handler_end  /*復帰処理へ*/
	
pv_handler_1:
	cmpi.l	#1,%d0      /*d0の値が1であるか比較*/
	bne	pv_handler_end  /*d0が1でないならば分岐*/
	jsr	v_body          /*v_body()を呼び出す*/
	
pv_handler_end:

	movem.l	(%sp)+,%d1  /*レジスタd1をスタックから復帰*/
	move.w	(%sp)+,%SR  /*SRをスタックから復帰*/
	movem.l (%sp)+, %a0-%a6/%d0-%d7
	rte                 /*割り込み終了*/         
