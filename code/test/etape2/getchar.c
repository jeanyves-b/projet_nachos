#include "syscall.h"

/*
 *  Programme de test de l'appel système GetChar
 */

int main()
{
	char c;
		c = GetChar();
		if (c!='\n') {
			PutString("Le caractère entré est: ");
			PutChar(c);
			PutChar('\n');
		}

	return 0;

}
