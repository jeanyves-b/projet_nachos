#include "syscall.h"

/*
 *  Programme de test de l'appel système GetInt
 */

int main()
{
	int i = GetInt();
	PutString("L'entier entre est :");
	PutInt(i);
	return 0;

}
