#include "syscall.h"

// Programme de tester qui lance deux processus en fork
void test(void *a) {
	SynchPutString("thread");
}

int main()
{
	//UserThreadCreate(test, 0);
	ForkExec("userpages0");
	ForkExec("userpages1");
	PutChar('\n');
	return 0;
}
