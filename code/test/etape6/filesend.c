/* filesend.c
 *	
 * 	Programme de test d'envoi de fichier
 *  Une autre machine doit exécuter en même temps le programme de test de réception de fichier
 *  Envoie vers la machine à l'adresse 1
 */

#include "syscall.h"
	int
main ()
{
	char file[100];
	PutString("Donner le nom du fichier à envoyer :");
	GetString(file, 99);
	Sleep(2);
	SendFile(file, 1, 1, 0);
	PutString("Fichier envoyé.");
	return 0;
}
