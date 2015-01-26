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
	PutString("Hellofffffffff");
	UserThreadCreate(thread2, 0);
	ForkExec("userpages");
	//ForkExec("userpages1");
	//ForkExec("user0");
	PutString("Hello");

	return 0;
}
