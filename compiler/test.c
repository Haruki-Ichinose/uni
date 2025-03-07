#include <stdio.h>

int main() {
    int x = 5;
    // (a) 文字定数のクオート忘れ（エラーを発生させる）
    // printf("%d\n", 'A + x');

    // (b) 演算子を忘れた場合
    // printf("%d\n", 'A' x);

    // (c) 未定義変数（xx を使う）
    // printf("%d\n", 'A' + xx);

    // (d) 足し算を引き算にする（正常動作）
    // printf("%d\n", 'A' - x);

    return 0;
}
