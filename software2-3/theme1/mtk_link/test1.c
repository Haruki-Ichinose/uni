#include <stdio.h>

int main()
{
  while(1){
    
    char str[256];
    scanf("%s",str);
    printf("%s\n",str);
    
    }
    return 0;
}


void exit(int value)
{
  *(char *)0x00d00039 = 'H'; /* LED0 への表示 (HALT) */
  for (;;) ; /* 無限ループトラップで停止させる */
}

