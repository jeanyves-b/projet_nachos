#include "syscall.h"

void forked()
{
	PutString("aaabbbcccdddeeefffggghhhiiijjjkkklllmmm");
}

int main()
{
	//UserThreadCreate(test, 0);
	//~ int i, pid[8];
	//~ for (i=0 ; i<8 ; i++)
		//~ pid[i] = ForkExec(forked());

	return 0;
}
