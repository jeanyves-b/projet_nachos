#include "syscall.h"
int main()
{
	int i;
	SynchGetInt(&i);
	SynchPutString("L'entier entre est :");
	SynchPutInt(i);
	return 0;

}
