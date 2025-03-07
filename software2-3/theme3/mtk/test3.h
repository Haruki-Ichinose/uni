#ifndef TEST3_H
#define TEST3_H

// カード枚数 (1〜13)
#define TOTAL_CARDS 13

// 「数字カード」だけを持つシンプルな構造体
typedef struct
{
    int number; // 1〜13
} CARD;

// グローバルな山札の配列 (13 枚)
extern CARD cards[TOTAL_CARDS];

// カードの初期化 (1〜13 を設定)
void init_card();

// カードのシャッフル
void shuffle_card();

#endif // TEST3_H
