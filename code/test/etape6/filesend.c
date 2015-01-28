/* filesend.c
 *	
 * 	Programme de test d'envoi du fichier 00_text (présent dans le répertoire build)
 *  Une autre machine doit exécuter en même temps le programme de test de réception de fichier
 *  Envoie vers la machine à l'adresse 1
 */

#include "syscall.h"
	int
main ()
{
	Sleep(2);
	PutInt(SendFile("00_text", 1, 1, 0));
	PutString("Fichier envoyé.");
	return 0;
}
