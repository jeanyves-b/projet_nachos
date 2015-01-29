/* filesend.c
 *	
 * 	Programme de test d'envoi du fichier 00_tiny (présent dans le répertoire build)
 *  Une autre machine doit exécuter en même temps le programme de test de réception de fichier
 *  Envoie vers la machine à l'adresse 1
 */

#include "syscall.h"
	int
main ()
{
	Sleep(2);
	PutInt(SendFile("network/outbox/00_tiny", 1, 1, 0));
	PutString("Fichier envoyé.");
	return 0;
}
