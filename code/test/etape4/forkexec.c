#include "syscall.h"

// Programme de tester qui lance deux processus en fork
void test(void *a) {
   PutString("On est dans le thread");
}

int main()
{
	UserThreadCreate(test, 0);
	int i,pid;
	for (i=0;i<19;i++)
		if ( (pid = ForkExec("user0")) < 0) {
			PutString("Impossible de crééer le processus.");
		}


	return 0;
}
