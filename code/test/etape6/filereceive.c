/* filereceive.c
 *	
 * 	Programme de test de reception de fichier
 *  Une autre machine doit exécuter en même temps le programme de test d'envoi de fichier
 */

#include "syscall.h"
	int
main ()
{
	ReceiveFile(0, "01_R");
	Sleep(2);
	PutString("Fichier reçu.");
	return 0;
}
