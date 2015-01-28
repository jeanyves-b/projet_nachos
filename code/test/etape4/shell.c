#include "syscall.h"

int main ()
{
	int result;
	int breakp = 1;
	char prompt[2], buffer[60];
	int i;

	prompt[0] = '-';
	prompt[1] = '-';

	while (breakp)
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
			if( (buffer[0] == 'q') && (i == 1)){
				breakp = 0;
			}else{
				result = ForkExec(buffer);
				if (result < 0){
					PutString("Commande inconnue\n");
				}
				while (GetNbP() > 1){
					Yield();
				}		
			}
				
		}
		
	}
	return 0;
}
