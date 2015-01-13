#include "syscall.h"

// Programme de tester qui lance deux processus en fork

int main()
{
	ForkExec("userpages0");
	ForkExec("userpages1");

	return 0;
}
