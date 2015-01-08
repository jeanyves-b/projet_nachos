#include "syscall.h"

/*
 *  Programme de test de l'appel système SynchPutChar
 */

void print(char c, int n)
{
  int i;
  for (i = 0; i < n; i++) {
    PutChar(c+i);
  }
  PutChar('\n');
}

int main()
{

print('a',4);
return 0;

}
