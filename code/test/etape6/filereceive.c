/* filereceive.c
 *	
 * 	Programme de test de reception de fichier
 *  Une autre machine doit exécuter en même temps le programme de test d'envoi de fichier
 */

#include "syscall.h"
	int
main ()
{
	char file[100];
	PutString("Donner l'emplacement où stocker le fichier:");
	GetString(file, 99);
	Sleep(2);
	ReceiveFile(1, file);
	PutString("Fichier reçu.");
	return 0;
}
