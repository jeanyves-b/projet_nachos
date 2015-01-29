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
	ForkExec("user0");
	ForkExec("user0");

	return 0;
}
