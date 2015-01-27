// filepost.cc 
//	Méthodes pour l'envoi et la réception de fichiers par réseau entre
//		deux machines.

#include "copyright.h"
#include "post.h"
#include "system.h"

//----------------------------------------------------------------------
// PostOffice::SendFile
//	Envoie la taille d'un fichier puis le fichier sur le réseau en le 
//	  fragmentant en plusieurs paquets si nécéssaire.
//
//	Renvoie 0 en cas de succès, un code d'erreur négatif sinon (-1 sur
//	  fichier impossible à ouvrir, -2 si taille du fichier invalide,
//	  -3 si tous les octets n'ont pas été acquittés. Pour ce dernier
//	   cas, certains octets ont été bien acquittés).
//
// "path" -- nom du fichier à envoyer
// "localPort" -- port local vers lequel les acquittement seront reçus
// "to" -- adresse du destinataire dans le réseau
// "remotePort" -- port distant (du destinataire) où envoyer le fichier
//----------------------------------------------------------------------

	int
PostOffice::SendFile(const char *path, int localPort, MailBoxAddress to, int remotePort)
{
	ASSERT((localPort >= 0) && (localPort < numBoxes));
	
	// ouverture du fichier
	OpenFile *file = fileSystem->Open(path);
	if (file == NULL)
		return -1;
		
	// récupération de la taille du fichier
	int size = file->Length();
	if (size <= 0)
		return -2;
		
	// on envoie la taille
	this->SendUnfixedSize((char*)(&size), sizeof(int), localPort, to, remotePort);

	// on envoie le contenu du fichier tant qu'il reste des données à envoyer
	int sent = 0;
	int toSendSent, toSend;
	char *buffer = new char[MaxStringSize];
	int remaining = size;
	
	// on découpe les parties envoyées en MaxStringSize
	do {
		if (remaining >= MaxStringSize)
			toSend = MaxStringSize;
		else
			toSend = remaining;
			
		toSendSent = 0;
		while (toSendSent < toSend) {
			toSendSent += file->Read(buffer + toSendSent, toSend - toSendSent);
		}
		remaining -= toSend;
		
		sent += this->SendUnfixedSize(buffer, (unsigned)(toSend), localPort, to, remotePort);
	} while (remaining > 0);
	
	delete [] buffer;
	
	// on n'a pas envoyé tout le fichier (paquets perdus)
	if (sent < file->Length()) {
		delete file;
		return -3;
	}
	
	delete file;
		
	return 0;
}
	
//----------------------------------------------------------------------
// PostOffice::ReceiveFile
//	Reçoit un fichier sur le port local indiqué et l'entregistre sous 
//	  le nom fourni.
//
//	Retourne 0 en cas de succès, un code négatif sinon (-1 si taille 
//	  reçue invalide, -2 si impossible de créer ou d'écraser le fichier
//	  au chemin souhaité, -3 si impossible d'ouvrir le fichier)
//
// "localPort" -- port local où la machine doit attendre un fichier
// "path" -- chemin où le fichier reçu doit être placé.
//----------------------------------------------------------------------

	int
PostOffice::ReceiveFile(int localPort, const char *path) {
		
	ASSERT((localPort >= 0) && (localPort < numBoxes));
	
	//	On reçoit la taille attendue
	unsigned size = 0;
	
	this->ReceiveUnfixedSize(localPort, (char*)(&size), sizeof(int));
	printf("my size is %d \n\n", size);
	// La taille du fichier est invalide
	if (size<=0) 
		return -1;
	
	// Impossible de créer une fichier (ou de l'écraser)
	if (!fileSystem->Create(path, size))
		return -2;
	
	OpenFile *file = fileSystem->Open(path);
	
	//	Impossible d'ouvrir le fichier
	if (file == NULL) 
		return -3;
	
	// On reçoit les données du fichier en fragments sur un buffer.
	char *buffer = new char[MaxStringSize];
	unsigned remaining = size;
	unsigned toReceive;
	
	// on découpe les parties reçues en MaxStringSize
	do {
		toReceive = (remaining >= MaxStringSize? MaxStringSize: remaining);
		
		this->ReceiveUnfixedSize(localPort, buffer, toReceive);
		remaining -= toReceive;
		
		// Ecriture du contenu du buffer sur le fichier.
		int written = 0;
		while ((unsigned)written < toReceive) {
			written += file->Write(buffer + written, toReceive - written);
		}
	} while (remaining > 0);
	delete [] buffer;	
	delete file;			

	return 0;
}
