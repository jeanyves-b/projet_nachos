#include "syscall.h"

/*
 *  Programme de test de l'appel système SynchGetChar
 */

int main()
{
	char c;
	while (c!='q') {
		c = SynchGetChar();
		if (c!='\n') {
			SynchPutString("Le caractère entré est: ");
			PutChar(c);
			PutChar('\n');
		}
	}

	return 0;

}
