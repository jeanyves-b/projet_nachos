#include "syscall.h"

/*
 *  Programme de test de l'appel système GetString
 */

int main()
{
	char s[10];
	GetString(s, 10);
	PutString("La chaine entree est :");
	PutString(s);
	return 0;

}
