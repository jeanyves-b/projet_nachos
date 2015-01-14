#include "syscall.h"

// /*
//  *  Programme de test userpages1
//  */

#define THIS "aaa"
#define THAT "bbb"
const int N = 10; // Choose it large enough!
void putss(char *s)
{
char *p; for (p = s; *p != '\0'; p++) PutChar(*p);
}
void f(void *s)
{
int i; for (i = 0; i < N; i++) putss((char *)s);
}
int main() {
UserThreadCreate(f, (void *) THIS);
f((void*) THAT);
return 0;
}
