#include "syscall.h"
int main()
{
	char s[10];
	SynchGetString(s, 10);
	SynchPutString("La chaine entree est :");
	SynchPutString(s);
	return 0;

}
