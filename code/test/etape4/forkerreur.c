#include "syscall.h"

// Programme de tester qui lance deux processus en fork

#define N 1000

int main()
{
	int i, pid;
	pid = ForkExec("papillon");
	if (pid == -1)
		PutString("test 1 : OK\n");
	else
		PutString("test 1 : NON\n");

	for (i=0 ; i<N ; i++)
	{
		pid = ForkExec("../halt")
		if(pid == -2 || pid == -4)
		{
			PutString("test 2 : OK\n");
			i = N;
		}
	}

	return 0;
}