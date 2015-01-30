#include "syscall.h"

/*
 *  Programme de test de l'appel système GetString
 */

int main()
{
	char s[50];
	GetString(s, 50);
	PutString("La chaine entree est :");
	PutString(s);
	return 0;

}
