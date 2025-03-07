***************************************************************
** スタック領域の確保
***************************************************************
.extern 	start
.global 	monitor_begin
.include	"equdefs.inc"

monitor_begin:

.section .bss
.even
SYS_STK: .ds.b 0x4000 	| システムスタック領域

.even
SYS_STK_TOP: 		| システムスタック領域の最後尾

.even
task_p:	.ds.l 1

***************************************************************
** 初期化
***************************************************************
.section .text
.even


boot:   ***************************************************
	** スーパーバイザ & 各種設定を行っている最中の割込禁止
	***************************************************
	move.w 	#0x2700,%SR
	lea.l	SYS_STK_TOP, %SP | Set SSP
	
	******************************
	** 割り込みコントローラの初期化
	*****************************
	move.b 	#0x40, IVR | ユーザ割り込みベクタ番号を0x40+level に設定．
	move.l 	#0x00ff3ffb,IMR | 全割り込みマスク
	
	******************************************************************
	** 送受信 (UART1) 関係の初期化 (割り込みレベルは 4 に固定されている)
	*****************************************************************
	move.w 	#0xe10c, USTCNT1 
	move.w 	#0x0038, UBAUD1 | baud rate = 230400 bps
	
	*********************************************************
	** タイマ関係の初期化 (割り込みレベルは 6 に固定されている)
	*********************************************************
	move.w 	#0x0004, TCTL1 

	move.l 	#uart1_interrupt, 0x110
	move.l 	#timer_interrupt, 0x118

	***************
	**キューの初期化
	***************
	jsr 	Init_Q
	
	move.l	#SYSTEM_CALL,0x080
	move.l	#0x00f3ff9, IMR
	
	
	move.w	#0x0000, %SR
	jmp	start
	


**************************
**送受信割り込みインターフェース
**************************
	
uart1_interrupt:
	movem.l %d0-%d3, -(%sp)      		/* レジスタの退避 */
	move.w 	UTX1, %d0			/* UTXの15ビット目が1かどうか見る */
	andi.l 	#0x8000, %d0
	beq 	uart1_interrupt_receive		/* 1以外なら分岐 */
	move.l 	#0, %d1				/* ch=0 */
	jsr 	INTERPUT			/* 1なら送信割り込み */
	
uart1_interrupt_receive:
	move.w 	URX1, %d3			/* URXの13ビット目が1かどうか見る */
	move.b	%d3, %d2			/* D3の下位8bit(データ部分)をD2.Bにコピー */
	andi.l	#0x2000, %d3
	beq	uart1_interrupt_end		/* 1以外なら分岐 */
	move.l 	#0, %d1				/* ch=0 */
	jsr 	INTERGET			/* 1なら受信割り込み */
	
uart1_interrupt_end:	
	movem.l (%sp)+, %d0-%d3			/* レジスタの回復 */
	rte					/* 呼び出し元に戻る */

	

***************************************
** タイマ用ハード割り込みインターフェース
***************************************
timer_interrupt:
    move.w 	TSTAT1, %d0        	/*TSTATをd0レジスタに読み込み*/
    andi.w	#0x01, %d0
    cmpi.w	#0, %d0			/*第0ビットを確認（1ならば割り込み発生）*/
    beq     	timer_interrupt_end	/*0ならば割り込み処理を終了*/
    move.w  	#0, TSTAT1        	/*TSTATをリセット*/
    jsr     	CALL_RP           	/*割り込み時に呼び出すルーチンを実行*/
timer_interrupt_end:
    rte                       		/*割り込み終了*/
    
    
****************************
** SYSTEM_CALL
** 入力
** d0:システムコール番号
** d1以降:システムコールの引数
** 出力
** d0:システムコール呼び出しの結果
****************************

SYSTEM_CALL:
	movem.l	%d1-%d3,-(%sp)		/*レジスタの退避*/
	cmp.l	#1,%d0
	bne	SYSTEM_CALL2		/*システムコール番号が１でないならば分岐*/
	jsr	GETSTRING		/*GETSTRINGを呼び出す*/
	bra	SYSTEM_CALL_FINISH
	
SYSTEM_CALL2:
	cmp.l	#2,%d0
	bne	SYSTEM_CALL3		/*システムコール番号が2でないならば分岐*/
	jsr	PUTSTRING		/*PUTSTRINGを呼び出す*/
	bra	SYSTEM_CALL_FINISH

SYSTEM_CALL3:
	cmp.l	#3,%d0
	bne	SYSTEM_CALL4		/*システムコール番号が3でないならば分岐*/
	jsr	RESET_TIMER		/*RESET_TIMERを呼び出す*/
	bra	SYSTEM_CALL_FINISH

SYSTEM_CALL4:
	cmp.l	#4,%d0
	bne	SYSTEM_CALL_FINISH	/*システムコール番号が4でないならば分岐*/
	jsr	SET_TIMER		/*SET_TIMERを呼び出す*/

SYSTEM_CALL_FINISH:
	movem.l	(%sp)+,%d1-%d3		/*レジスタの復帰*/
	rte
    
    
***************************
**キューの初期化処理
***************************
Init_Q:
	jsr 	Init_Q0			/* 受信用キューを初期化 */
	jsr 	Init_Q1			/* 送信用キューを初期化 */
	rts				/* 呼び出し元に復帰 */

Init_Q0:
	lea.l	Queue0, %a1		/* Queue0領域の先頭アドレス -> A1*/
	move.l	%a1, %a2		/* Queue0のバッファの最終アドレス -> A2*/
	add.l	#BF_END, %a2		
	move.l	%a1, top(%a1)		/* topを初期化 */
	move.l	%a1, out(%a1)		/* outを初期化 */
	move.l	%a1, in(%a1)		/* inを初期化 */
	move.l	%a2, bottom(%a1)	/* bottomを初期化 */
	move.b	#0x00, s(%a1)		/* sを初期化 */
	rts

Init_Q1:	/* Q0と同様にQ1を初期化 */
	lea.l	Queue1, %a1
	move.l	%a1, %a2
	add.l	#BF_END, %a2
	move.l	%a1, top(%a1)
	move.l	%a1, out(%a1)
	move.l	%a1, in(%a1)
	move.l	%a2, bottom(%a1)
	move.b	#0x00, s(%a1)
	rts

****************************
** SelectQueue
** 入力
** d0: 受信(0)or 送信(1)
** 出力
** a1: 選択したキューの先頭アドレス
****************************
SelectQueue:
	movea.l	#Queue0,%a1		/* d0が0なら#Queue0 -> a1 */
	cmp	#0, %d0				
	beq	End_SelectQueue
	movea.l	#Queue1,%a1		/* d0が1なら#Queue1 -> a1 */
End_SelectQueue:
	rts				/* 呼び出し元に復帰 */	

*************************
** INQ
** キューに要素を入れる
*************************	
INQ:
	move.w	 %SR, -(%sp)		/* 現走行レベルの退避 */
	move.w	#0x2700, %SR		/* 割り込み禁止（=走行レベルを7に）*/
	movem.l	%a0-%a3/%d2, -(%sp)	/*レジスタの退避*/
	jsr	SelectQueue		/*SelectQueueで編集するキューの先頭をa1へ*/
	move.l	#0, %d0			/*D0はPUT_BUF内で失敗判定に使用*/
	jsr	PUT_BUF			/* PUT_BUFの呼び出し */
	movem.l	(%sp)+, %a0-%a3/%d2	/*レジスタの回復*/
	move.w 	(%sp)+, %SR		/* 走行レベルの回復 */
	rts				/* 割り込み終了 */

*************************
** OUTQ
** キューから要素を取り出す
** 入力
** d0: 受信(0)or 送信(1) for SelectQueue
*************************
OUTQ:
	move.w 	%SR, -(%sp)		/* 現走行レベルの退避 */
	move.w	#0x2700, %SR		/* 割り込み禁止（=走行レベルを7に）*/
	movem.l	%a0-%a3/%d2, -(%sp)	/*レジスタの退避*/
	jsr	SelectQueue		/*SelectQueueで編集するキューの先頭をa1へ*/
	move.l	#0, %d0			/* D0はGET_BUF内で失敗判定に使用*/
	jsr	GET_BUF			/* GET_BUFの呼び出し */
	movem.l	(%sp)+, %a0-%a3/%d2	/*レジスタの回復*/
	move.w 	(%sp)+, %SR		/* 走行レベルの回復 */
	rts				/* 割り込み終了 */

*************************
** PUT_BUF
** 入力
** a1: キューの先頭アドレス
** a2: 比較用アドレスレジスタ
** a3: キュー内データの格納先アドレス
** d1: キューに格納するデータ
*************************
PUT_BUF:
	move	s(%a1), %d2		/* 要素数 -> d2 */
	cmp	 #0x100, %d2		/* キューが満杯なら何もせず復帰 */
	beq	END_PUT_BUF
	movea.l in(%a1), %a3		/* キューにデータを格納  */
	move.b	%d1, (%a3)
	movea.l	bottom(%a1), %a2	/* 格納先がデータ領域の最後なら先頭へ */
	cmpa.l	in(%a1), %a2
	beq	STEP1_PUT_BUF
	add.l	#1, in(%a1)		/* そうでないなら単に次へ進める */
	bra	STEP2_PUT_BUF

STEP1_PUT_BUF:
	move.l	top(%a1), in(%a1)

STEP2_PUT_BUF:
	addq	#1, s(%a1)		/* 要素数 s = +1 */
	move.l	#1, %d0			/* 書き込み成功 */

END_PUT_BUF:	
	rts				/* 呼び出し元への復帰 */
	
****************************
** GET_BUF
** a1: キューの先頭アドレス
** a2: 比較用アドレスレジスタ
** a3: キュー内データの読み出し元アドレス
***************************	
GET_BUF:
	move	s(%a1), %d2		/* キューが空かどうか確認 */
	cmp	#0x0, %d2
	beq	END_GET_BUF		/* キューが空なら何もせず復帰 */
	movea.l out(%a1), %a3		/* キューから取り出したデータ -> d1 */
	move.b	(%a3), %d1			
	movea.l	bottom(%a1), %a2	/* キュー領域の最後からデータを取り出したら次の取り出し先を先頭へ */
	cmpa.l	out(%a1), %a2
	beq	STEP1_GET_BUF
	add.l	#1, out(%a1)		/* そうでないなら単に次へ進める */
	bra	STEP2_GET_BUF

STEP1_GET_BUF:
	move.l	top(%a1), out(%a1)


STEP2_GET_BUF:
	subq	#1, s(%a1)		/* 要素数 s = -1 */
	move.l	#1, %d0			/* 書き込み成功 */

END_GET_BUF:	
	rts				/* 呼び出し元への復帰 */

**********************
** INTERPUT
** 入力: ch ->　d1
**********************
INTERPUT:
	movem.l %a0-%a3, -(%sp)		/* レジスタの退避 */
	move.w 	%SR, -(%sp)
	move.w 	#0x2700, %SR		/*走行レベル7に設定*/

	cmpi.b 	#0, %d1
	bne 	END_INTERPUT		/*ch!=0なら何もせず復帰*/

	moveq.l #1, %d0
	jsr 	OUTQ			/*OUTQ(1,%d1)を実行*/
	
	cmpi.b 	#0, %d0
	bne 	OUTQ_FAIL		/*OUTQが失敗なら送信割り込みをマスク*/

	move.w 	#0xe108, USTCNT1	/*送信割り込みをマスク*/
	bra 	END_INTERPUT

OUTQ_FAIL:
	add.w 	#0x800, %d1		/*ヘッダ付与*/
	move.w 	%d1, UTX1		/*UTX1に送信*/

END_INTERPUT:
	move.w 	(%sp)+, %SR		/* レジスタの回復 */
	movem.l (%sp)+, %a0-%a3
	rts				/* 割り込み終了 */


******************
**PUTSTRING
** 入力:
**	ch ->　d1
**	p  ->　d2
**	size ->　d3
** 出力:
**	実際送信したデータ数->d0 
******************

PUTSTRING:
	movem.l %d1-%d4/%a0, -(%sp)		/* レジスタの退避 */
	cmpi.b 	#0, %d1				/* chが0以外なら何もせず復帰 */
	bne 	END_PUTSTRING
	move.l	#0, %d4				/* %d4を実際送信したデータ数のカウンタとして使用 */
	move.l 	%d2, %a0			/* pをa0にコピー */
	cmpi.b 	#0, %d3				/* size=0なら d0=0 にして復帰 */
	beq 	PUTSTRING_STEP3

PUTSTRING_STEP1:
	cmp 	%d4, %d3			/* カウンタとsizeを比較 */
	beq 	PUTSTRING_STEP2			/* イコールなら分岐 */
	move.l	 #1, %d0			/* 送信キューを指定 */
	move.b 	(%a0)+, %d1			/* データをコピーしてa0をインクリメント */
	jsr 	INQ				/* INQを呼び出す */
	cmp 	#0, %d0
	beq 	PUTSTRING_STEP2
	add 	#1, %d4
	bra 	PUTSTRING_STEP1			/* ループを回す */

PUTSTRING_STEP2:
	move.w 	#0xe10c, USTCNT1		/* USTCNT1を操作して送信割り込み許可*/

PUTSTRING_STEP3:
	move.l 	%d4, %d0			/* sz -> d0 */

END_PUTSTRING:
	movem.l (%sp)+, %d1-%d4/%a0		/* レジスタの回復 */
	rts					/* 呼び出し元への復帰 */
    
*******************
** INTERGET
** 入力: ch->d1
**	 data->d2
******************

INTERGET:
	movem.l	%d0-%d2, -(%sp)		/*レジスタの退避*/
	cmpi.b	#0, %d1
	bne	END_INTERGET		/*ch=0なら何もせず復帰*/
	move.l	#0x0, %d0
	move.l	%d2, %d1
	jsr	INQ			/*OUTQ(1,%d1)を実行*/

END_INTERGET:
	movem.l	(%sp)+, %d0-%d2		/*レジスタの回復*/
	rts


************
** GETSTRING
** 入力:ch -> d1
**	p  -> d2 /* データ書き込み先の先頭アドレス*/
**	size ->d3 /*取り出すデータサイズ*/
** 戻り値: sz ->d0	
*************

GETSTRING:
	movem.l	%d1-%d4/%a0, -(%sp)		/*レジスタの退避*/
	cmpi.b	#0, %d1				/*chが0以外なら何もしない*/
	bne	END_GETSTRING
	move.l	#0, %d4				/*%d4をszとして使用*/
	movea.l	%d2, %a0			/*%a0をiとして使用*/
	
GETSTRING_LOOP:	
	cmp	%d3, %d4			/*sz = sizeならば分岐*/
	beq	END_GETSTRING	
	move.l	#0, %d0				/*受信キューを指定*/
	jsr	OUTQ				/*OUTQにより受信キューからデータ読み込み*/
	cmpi.b	#0, %d0				/*OUTQが失敗ならば分岐*/
	beq	END_GETSTRING
	move.b	%d1, (%a0)+			/*i番地にdataをコピーし、i++*/
	addq.l	#1, %d4				/*sz++*/
	bra	GETSTRING_LOOP

END_GETSTRING:
	move.l	%d4, %d0			/*sz -> %d0*/
	movem.l	(%sp)+, %d1-%d4/%a0		/*レジスタの回復*/
	rts

**********************
** RESET_TIMER
** TCTLを割り込み不可に設定
**********************
	
RESET_TIMER:
	move.w	#0x0004, TCTL1			/* TCTLを割り込み不可*/
	rts


	
**********************
** SET_TIMER
** 入力
** タイマ割り込み発生周期t -> %d1	
** 割り込み時に起動するルーチンの先頭アドレス-> %d2
**********************
	
SET_TIMER:
	movem.l	%d1-%d2, -(%sp)	/* レジスタ退避 */
	move.l	%d2, task_p	/* 先頭アドレスをtask_pへ代入 */
	move.w	206, TPRER1	/* TPRERを206に設定 */
	move.w	%d1, TCMP1	/* 割り込み発生周期をTCMPへ */
	move.w	#0x0015, TCTL1	/* TCTLを割り込み許可 */
	movem.l	(%sp)+, %d1-%d2	/* レジスタの回復 */
	rts


****************************
** CALL_RP
** タイマ割り込み時に処理すべきルーチンを呼び出す
****************************

CALL_RP:
	movem.l	%a0, -(%sp)
	movea.l	(task_p), %a0
	jsr	(%a0)		/* task_pが指すアドレスへジャンプ */
	movem.l	(%sp)+, %a0
	rts

.end



