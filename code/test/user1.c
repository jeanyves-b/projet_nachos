#include "syscall.h"

/*
 *  Programme de test userpages1
 */


int main()
{
	PutString("On est dans le premier processus.");
 	int i = 0;
 	while (i++ < 600);
 	PutString("On sort du premier processus.");
	return 0;
}
