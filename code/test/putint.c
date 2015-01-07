#include "syscall.h"
int main()
{
	int i;
	for (i = -2; i <= 14; i++)
		SynchPutInt(i);
	return 0;

}
