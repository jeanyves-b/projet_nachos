/* ring_i.c
 *	
 * 	Programme de test de element intermedaire d'un anneau
 *  Une autre machine doit exécuter en même temps au moins ring_0 et ring_last
 *  A executer avec -m i avec i > 0
 */

#include "syscall.h"
#define SIZE 53
	int
main ()
{
	
	char got[SIZE];
	Receive(0, got, SIZE);
	PutString("chaine reçue: ");
	PutString(got);
	
	int from = (got[0]-'0')*10 + got[1]-'0';
	
	got[0] = (from+1) / 10 + '0';
	got[1] = (from+1) % 10 + '0';
	
	int i = 3;
	while (i< SIZE-1) {
		if (got[i] == 'Z')
			got[i] = 'A';
		else
			got[i]++;
		i++;
	}
	got[SIZE-1] = '\0';
	
	Send(got, SIZE, 1, from+2, 0);
	PutString("chaine envoyée:");
	PutString(got);
	
	
	return 0;
}
