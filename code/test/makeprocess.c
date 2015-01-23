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
	ForkExec("user0");
	PutString("Hello");
	return 0;
}
