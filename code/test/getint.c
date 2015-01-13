#include "syscall.h"

/*
 *  Programme de test de l'appel système SynchGetInt
 */

int main()
{
	int i;
	SynchGetInt(&i);
	SynchPutString("L'entier entre est :");
	SynchPutInt(i);
	return 0;

}
