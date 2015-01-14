#include "syscall.h"

// Programme de tester qui lance deux processus en fork
/* void test(void *a) {
	SynchPutString("thread");
} */

int main()
{
	//UserThreadCreate(test, 0);
	
	 int i, pid;
	for (i=0;i<3;i++)
		if ( (pid = ForkExec("userpages0")) < 0)
			PutChar('}');
	//ForkExec("userpages1");
	SynchPutString("Finished main"); 
	return 0;
}
