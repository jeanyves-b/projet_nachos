#include "syscall.h"

// Programme de tester qui lance deux processus en fork
/* void test(void *a) {
   PutString("thread");
   } */

int main()
{
	//UserThreadCreate(test, 0);

	int i, pid;
<<<<<<< HEAD
	for (i=0;i<8;i++)
		if ( (pid = ForkExec("userpages")) < 0)
			PutChar('}');
=======
	for (i=0;i<8;i++){
	  pid = ForkExec("user0");
	  PutString("here");
	  PutInt(pid);
	  if (pid >= 0)
	    PutChar('}');
	}
>>>>>>> 3e9c7db9efac9f81b2f7078d22c4d9325a0e8bb3
	//ForkExec("userpages1");

	return 0;
}
