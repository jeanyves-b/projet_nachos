#include "syscall.h"

int main ()
{
	int newProc;
	char prompt[2], buffer[60];
	int i;

	prompt[0] = '-';
	prompt[1] = '-';

	while (1)
	{
		for (i=0;i<2;i++)
		  PutChar (prompt[i]);

		i = 0;
		do
		{
			buffer[i] = GetChar ();
		}
		while (buffer[i++] != '\n');
		buffer[--i] = '\0';
		if (i > 0)
		{
			newProc = ForkExec(buffer);
		}
		while (GetNbP() > 1){
			Yield();
		}
	}
}
