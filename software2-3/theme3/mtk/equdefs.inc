***********************************************
** 変数の定義
***********************************************

***************************************************************
** 各種レジスタ定義
***************************************************************
***************
** レジスタ群の先頭
***************
.equ REGBASE, 	0xFFF000 	| DMAP を使用．
.equ IOBASE, 	0x00d00000
	
***************
** 割り込み関係のレジスタ
***************
.equ IVR, REGBASE+0x300 	| 割り込みベクタレジスタ
.equ IMR, REGBASE+0x304 	| 割り込みマスクレジスタ
.equ ISR, REGBASE+0x30c 	| 割り込みステータスレジスタ
.equ IPR, REGBASE+0x310 	| 割り込みペンディングレジスタ
	
***************
** タイマ関係のレジスタ
***************
.equ TCTL1, 	REGBASE+0x600 	| タイマ１コントロールレジスタ
.equ TPRER1, 	REGBASE+0x602 	| タイマ１プリスケーラレジスタ
.equ TCMP1, 	REGBASE+0x604 	| タイマ１コンペアレジスタ
.equ TCN1, 	REGBASE+0x608 	| タイマ１カウンタレジスタ
.equ TSTAT1, 	REGBASE+0x60a 	| タイマ１ステータスレジスタ
	
***************
** UART1（送受信）関係のレジスタ
***************
.equ USTCNT1, 	REGBASE+0x900 	| UART1 ステータス/コントロールレジスタ
.equ UBAUD1, 	REGBASE+0x902 	| UART1 ボーコントロールレジスタ
.equ URX1, 	REGBASE+0x904 	| UART1 受信レジスタ
.equ UTX1, 	REGBASE+0x906 	| UART1 送信レジスタ
	
***************
** LED
***************
.equ LED7, IOBASE+0x000002f
.equ LED6, IOBASE+0x000002d
.equ LED5, IOBASE+0x000002b
.equ LED4, IOBASE+0x0000029
.equ LED3, IOBASE+0x000003f
.equ LED2, IOBASE+0x000003d
.equ LED1, IOBASE+0x000003b
.equ LED0, IOBASE+0x0000039

***************
** システムコール番号
***************
.equ SYSCALL_NUM_GETSTRING, 	1
.equ SYSCALL_NUM_PUTSTRING, 	2
.equ SYSCALL_NUM_RESET_TIMER, 	3
.equ SYSCALL_NUM_SET_TIMER, 	4

****************************************************************
*** 初期値のあるデータ領域
****************************************************************
.section .data

.even 				| \n: 次の行へ (ラインフィード)
TTC:	.dc.w 0
.even
	
****************************************************************
*** 初期値の無いデータ領域
****************************************************************
.section .bss
BUF: .ds.b 256 			| BUF[256]

.even
USR_STK: .ds.b 0x4000 		| ユーザスタック領域
.even
USR_STK_TOP: 			| ユーザスタック領域の最後尾
	
*******************************
.section .bss
Queue0:		.ds.b	274	/* 受信用キュー */
Queue1:		.ds.b	274	/* 送信用キュー */

.section .text
.equ	BF_START, 	0		/* キューのデータ領域の先頭 */
.equ	BF_END, 	255		/* キューのデータ領域の最後尾 */
.equ	top, 		256		/* キューの先頭アドレスを保持へのオフセット */
.equ	out, 		260		/* 次にpopするデータアドレスを保持へのオフセット */
.equ	in, 		264		/* 次にinするデータアドレスを保持へのオフセット */
.equ	bottom, 	268		/* キューの最終アドレスを保持へのオフセット */
.equ	s, 		272		/* キュー内の要素数を保持する変数へのオフセット */



