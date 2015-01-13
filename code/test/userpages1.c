#include "syscall.h"
// /*
//  *  Programme de test userpages1
//  */
//  
//  void thread1(void *n){
// 	 int i=0;
// 	 while (i<10) {
// 		PutChar('a'+i);
// 		i++;
// 	}
// }
// 
//  void thread2(void *n){
// 	 int i=0;
// 	 while (i<10) {
// 		PutChar('0'+i);
// 		i++;
// 	}
// }
// 
// int main()
// {
// 
// 	unsigned id = UserThreadCreate(thread1, 0);
// 	unsigned id2 = UserThreadCreate(thread2, 0);
// 	int error;
// 	
// 	if (id >= 0){
// 		error = UserThreadJoin(id); 
// 		if (error < 0){
// 			SynchPutString("erreur Thread 1");
// 		}
// 	}
// 	if (id2 >= 0){
// 		error = UserThreadJoin(id2);
// 		if (error < 0){
// 			SynchPutString("erreur Thread 2");
// 		}
// 	} 
// 	PutChar('\n');
// 	return 0;
// 

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
