#include "syscall.h"

/*
 *  Programme de test de l'appel système SynchGetChar
 */

int main()
{
char c;
while (c!='q') {
	c = SynchGetChar();
	SynchPutString("Le caractère entré est: ")
	PutChar(c);
}
	
return 0;

}
