#include "syscall.h"

void thread2(void *n){
	int i=0;
	while (i<10) {
		PutChar('0'+i);
		i++;
	}
}


int main()
{
<<<<<<< HEAD
	UserThreadCreate(thread2, 0);
	ForkExec("user");
	//ForkExec("userpages1");
=======
	ForkExec("user0");
	PutString("Hello");
>>>>>>> 3e9c7db9efac9f81b2f7078d22c4d9325a0e8bb3
	return 0;
}
