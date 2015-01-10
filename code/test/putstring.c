#include "syscall.h"

/*
 *  Programme de test de l'appel système SynchPutString
 */

int main()
{

	//Tests affichage d'une chaine simple
	SynchPutString("je suis la chaine");

	//Test affichage des accents
	SynchPutString("ùôéêêẽëà");

	//Test d'une longue chaine qui contient \n (l'affichage s'arrête au \n)
	SynchPutString("je suis une très très très\ntrès très très très longue chaine de caractères");
	return 0;

}
