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
	UserThreadCreate(thread2, 0);
	ForkExec("userpages0");
	ForkExec("userpages1");
	return 0;
}
