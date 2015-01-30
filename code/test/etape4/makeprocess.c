#include "syscall.h"


int main()
{
	ForkExec("user1");
	ForkExec("user2");

	return 0;
}
