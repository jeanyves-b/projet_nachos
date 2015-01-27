/* ring_last.c
 *	
 * 	Programme de test de dernier element d'un anneau
 *  Une autre machine doit exécuter en même temps au moins ring_0 (voire ring_i)
 *  A executer avec -m i avec i >0
 */

#include "syscall.h"
#define SIZE 50
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
		got[i]++;
		i++;
	}
	got[SIZE-1] = '\0';
	
	Send(got, SIZE, 1, 0, 0);
	PutString("chaine envoyée:");
	PutString(got);
	
	
	return 0;
}
