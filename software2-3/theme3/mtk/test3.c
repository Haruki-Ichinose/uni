#include <stdio.h>
#include "mtk_c.h"
#include "test3.h"

CARD cards[TOTAL_CARDS];

// -------------------- グローバル変数 --------------------
int round_count;
int top_index;

//int score[2];
int score1;
int score2;
//char guess[2];
int guess1;
int guess2;

int is_finish;
int winner;
volatile int flg0 = 0; 
volatile int flg1 = 0; // プレイヤー入力完了フラグ

// -------------------- 乱数生成 --------------------
volatile unsigned int rand_counter = 2;

void random_generator()
{
    while (1)
    {
        rand_counter = (rand_counter * 1103515245 + 12345) & 0x7FFFFFFF;
    }
}

unsigned int get_random()
{
    return rand_counter;
}

// -------------------- カード初期化 --------------------
void init_card()
{
    for (int i = 0; i < TOTAL_CARDS; i++)
    {
        cards[i].number = i + 1;
    }
}

void shuffle_card()
{
    for (int i = TOTAL_CARDS - 1; i > 0; i--)
    {
        unsigned int r = get_random() % (i + 1);
        int temp = cards[i].number;
        cards[i].number = cards[r].number;
        cards[r].number = temp;
    }
}

// -------------------- ラウンド表示 --------------------
void first_display_round()
{
        fprintf(com0out, "----------------------\n");
        fprintf(com0out, "Round %d\n", round_count);
        fprintf(com0out, "Current Number: %d\n", cards[top_index].number);
        fprintf(com0out, "Player0 Score=%d, Player1 Score=%d\n", score1, score2);

        fprintf(com1out, "----------------------\n");
        fprintf(com1out, "Round %d\n", round_count);
        fprintf(com1out, "Current Number: %d\n", cards[top_index].number);
        fprintf(com1out, "Player0 Score=%d, Player1 Score=%d\n", score1, score2);
}

// -------------------- Player 0 --------------------
void player0(){
    P(0);
    while (1)
    {
        fprintf(com0out, "Player 0: 次のカードは High(1) か Low(0) か? (Score=%d): ", score1);

        int buf1;
        fscanf(com0in, "%d", &buf1);
        
    if (buf1 == 1){	//High
        guess1 = 1;}
    else if (buf1 == 0){	//Low
        guess1= -1;}
    else{
        guess1 = 1;		//default High
        }
        
        V(0);
        V(2);
        P(3);
    }
}

// -------------------- Player 1 --------------------
void player1(){
    P(1);
    while (1)
    {
        fprintf(com1out, "Player 0: 次のカードは High(1) か Low(0) か? (Score=%d): ", score2);

        int buf2;
        fscanf(com1in, "%d", &buf2);
        
    if (buf2 == 1){	//High
        guess2 = 1;}
    else if (buf2 == 0){	//Low
        guess2= -1;}
    else{
        guess2 = 1;		//default High
        }
        
        V(1);
        V(2);
        P(3);
    }
}

// -------------------- 結果判定 --------------------
void judgment()
{
    while (1)
    {
        P(0);
        P(1);
        P(2);
        P(2);
        int next_number = cards[++top_index].number;
        if (next_number > cards[top_index - 1].number)
        {
            if (guess1== 1)
                score1++;
            if (guess2 == 1)
                score2++;
        }
        else if (next_number < cards[top_index - 1].number)
        {
            if (guess1 == -1)
                score1++;
            if (guess2 == -1)
                score2++;
        }

        round_count++;

        fprintf(com0out, "----------------------\n");
        fprintf(com0out, "Round %d\n", round_count);
        fprintf(com0out, "Current Number: %d\n", cards[top_index].number);
        fprintf(com0out, "Player0 Score=%d, Player1 Score=%d\n", score1, score2);

        fprintf(com1out, "----------------------\n");
        fprintf(com1out, "Round %d\n", round_count);
        fprintf(com1out, "Current Number: %d\n", cards[top_index].number);
        fprintf(com1out, "Player0 Score=%d, Player1 Score=%d\n", score1, score2);

        if (round_count >= 5 || top_index >= TOTAL_CARDS)
        {
            is_finish = 1;
            winner = (score1 > score2)   ? 0
                     : (score1 < score2) ? 1
                                             : -1;

            fprintf(com0out, "*** Game Over! ***\nWinner: %s\n",
                    (winner == -1)  ? "Draw"
                    : (winner == 0) ? "Player 0"
                                    : "Player 1");

            fprintf(com1out, "*** Game Over! ***\nWinner: %s\n",
                    (winner == -1)  ? "Draw"
                    : (winner == 0) ? "Player 0"
                                    : "Player 1");
            while(1){
            }
                                    
            
        }
        V(3);
        V(3);
        V(0);
        V(1);
    }
}

//-------------------------------------------
// mapping()
//-------------------------------------------
void mapping()
{
    com0in  = fdopen(3, "r");
    com0out = fdopen(3, "w");
    com1in  = fdopen(4, "r");
    com1out = fdopen(4, "w");
}

// -------------------- main関数 --------------------
int main()
{
    mapping();
    init_kernel();
    init_card();
    shuffle_card();
    first_display_round();
	
    semaphore[0].count = 1;
    semaphore[1].count = 1;
    semaphore[2].count = 0;
    semaphore[3].count = 0;

    set_task((char*)random_generator);
    set_task((char*)player0);
    set_task((char*)player1);
 //   set_task((char*)first_display_round);
    set_task((char*)judgment);

    begin_sch();
    return 0;
}
