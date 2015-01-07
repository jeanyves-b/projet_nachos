#include "syscall.h"

/*
 *  Programme de test de l'appel système SynchPutInt
 */

int main()
{
	//Affichage des entiers de -2 à 11
	int i;
	for (i = -2; i <= 11; i++)
		SynchPutInt(i);
		
	//Affichage d'un entier très grand
	SynchPutInt(1234567890);
	
	return 0;

}
