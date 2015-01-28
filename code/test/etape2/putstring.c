#include "syscall.h"

/*
 *  Programme de test de l'appel système PutString
 */

int main()
{

	//Tests affichage d'une chaine simple
	PutString("je suis la chaine");

	//Test affichage des accents
	PutString("ùôéêêẽëà");

	//Test d'une longue chaine qui contient \n (l'affichage s'arrête au \n)
	PutString("je suis une très très très\ntrès très très très longue chaine de caractères");
	PutString("je suis une très très très très très très très longue chaine de caractères");
	return 0;

}
