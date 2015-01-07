#include "syscall.h"
void print(char c, int n)
{
  int i;
  char s[n+1];
  for (i = 0; i < n; i++) {
    //PutChar(c+i);
    s[i] = c+i;
  }
  //PutChar('\n');
  //s[24]='a';
  SynchPutString(s);
}

int main()
{
  print('a',26);
  //Halt();
  return 0;
}
