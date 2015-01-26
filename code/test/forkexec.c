#include "syscall.h"

// Programme de tester qui lance deux processus en fork
/* void test(void *a) {
   PutString("thread");
   } */

int main()
{
	//UserThreadCreate(test, 0);

	int i, pid;

	for (i=0;i<8;i++)
		if ( (pid = ForkExec("userpages")) < 0)
			PutChar('}');
		else
			PutString("fork reussi\n");
	

/*	for (i=0;i<8;i++){
	  pid = ForkExec("user0");
	  PutString("here");
	  PutInt(pid);
	  if (pid >= 0)
	    PutChar('}');
	}
*/
	//ForkExec("userpages1");

	return 0;
}
