#include "syscall.h"

// Programme de tester qui lance deux processus en fork

#define N 1000

int main()
{
	int i, pid;
	pid = ForkExec("papillon");
	if (pid == -1)
		PutString("Test programme inexistant: OK\n");
	else
		PutString("Test programme inexistant : NON\n");

	for (i=0 ; i<N ; i++)
	{
		pid = ForkExec("nothing");
		if(pid == -2 || pid == -3)
		{
			PutString("Test trop grand nombre de processus : OK\n");
			i = N;
		}
			
	}
	return 0;
}
