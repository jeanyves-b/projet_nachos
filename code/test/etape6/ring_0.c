/* ring_0.c
 *	
 * 	Programme de test de prremier element d'un anneau
 *  Une autre machine doit exécuter en même temps au moins ring_last (voire ring_i)
 *  A executer avec -m 0
 */

#include "syscall.h"
#define SIZE 50
	int
main ()
{
	char send[SIZE];
	
	send[0] = '0';
	send[1] = '0';
	send[2] = '_';
	
	int i = 3;
	while (i< SIZE-1) {
		send[i] = 'A' + i%26;
		i++;
	}
	send[SIZE-1] = '\0';
	
	Send(send, SIZE, 1, 1, 0);
	PutString("chaine envoyée:");
	PutString(send);
	
	char got[SIZE];
	Receive(0, got, SIZE);
	PutString("chaine reçue: ");
	PutString(got);
	return 0;
}
