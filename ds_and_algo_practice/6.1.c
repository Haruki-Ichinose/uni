#include <stdio.h>
#include <string.h>

void Table1(int table1[], char pattern[], int pattern_len)
{

    for (int k = 0; k < 256; k++)
    {
        table1[k] = pattern_len;
    }

    for (int pos = 0; pos < pattern_len - 1; pos++)
    {
        table1[pattern[pos]] = pattern_len - 1 - pos;
    }
}

int bmSimpleSearch(char text[], int text_len, char pattern[], int pattern_len)
{
    int text_pos;
    int pattern_pos;
    int table1[256];

    Table1(table1, pattern, pattern_len);

    pattern_pos = pattern_len - 1;
    text_pos = pattern_len - 1;

    while (text_pos < text_len)
    {
        if (text[text_pos] == pattern[pattern_pos])
        {
            /* 一致した場合 */
            if (pattern_pos == 0)
            {
                return text_pos;
            }
            text_pos--;
            pattern_pos--;
        }
        else
        {
            /* 不一致だった場合 */
            if (table1[text[text_pos]] > (pattern_len - 1) - pattern_pos)
            {
                text_pos += table1[text[text_pos]];
            }
            else
            {
                text_pos += (pattern_len - 1) + pattern_pos + 1;
            }
            pattern_pos = (pattern_len - 1);
        }
    }
    return -1;
}

int main()
{
    char text[] = "The quick brown fox jumps over the lazy dog";
    char pattern[] = "lazy";
    int text_len = strlen(text);
    int pattern_len = strlen(pattern);
    int result;

    result = bmSimpleSearch(text, text_len, pattern, pattern_len);

    if (result == -1)
    {
        printf("パターンが見つかりませんでした\n");
    }
    else
    {
        printf("パターンが見つかりました\n");
        printf("テキストの位置: %d\n", result);
    }

    return 0;
}
