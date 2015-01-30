#include "syscall.h"

/*
 *  Programme de test userpages2
 */


int main()
{
	PutString("On est dans le second processus.");
 	int i = 0;
 	while (i++ < 600);
 	PutString("On sort du second processus.");
	return 0;
}
