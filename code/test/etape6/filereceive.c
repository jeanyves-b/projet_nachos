/* filereceive.c
 *	
 * 	Programme de test de reception de fichier
 *  Une autre machine doit exécuter en même temps le programme de test d'envoi de fichier
 */

#include "syscall.h"
	int
main ()
{
	Sleep(2);
	ReceiveFile(0, "network/inbox/01_R");
	
	PutString("Fichier reçu.");
	return 0;
}
