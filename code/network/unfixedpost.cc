// unfixedpost.cc 
//	Méthodes pour l'envoi et la réception de fichiers par réseau entre
//		deux machines.

#include "copyright.h"
#include "post.h"
#include "system.h"
//----------------------------------------------------------------------
// PostOffice::SendUnfixedSize
//	Envoi des données sans limite de taille en les fragmentant en 
//	  pluseurs paquets en utilisant l'envoi synchrone sur un certain
//	  port de la machine desitante, et en attendant les acquittements
//	  sur un certain port local.
//	Un paquet a une taille maximum de "MaxMailSize" et l'envoi est
//	  est synchrone. 
//
//	Renvoie le nombre d'octets acquittés.
//	
// "data" -- données à envoyer
// "size" -- taille des données à envoyer
// "localPort" -- port local vers lequel les acquittement seront reçus
// "to" -- adresse du destinataire dans le réseau
// "remotePort" -- port distant (du destinataire) où envoyer les données
//----------------------------------------------------------------------
	unsigned
PostOffice::SendUnfixedSize(char* data, unsigned size, int localPort, 
		MailBoxAddress to, int remotePort)
{
	ASSERT(size <= MaxStringSize);
	
	PacketHeader *pktHdr = new PacketHeader();
	MailHeader *mailHdr = new MailHeader();
	pktHdr->to = to;		
	mailHdr->to = remotePort;
	mailHdr->from = localPort;
	mailHdr->length = size;
	
	if (DebugIsEnabled('n')) {
		printf("Sending message with size %d: ",size);
		PrintHeader(*pktHdr, *mailHdr);
	}
	
	ASSERT(0 <= mailHdr->to && mailHdr->to < numBoxes);

	unsigned bytesSent = 0;
	unsigned bytesTriedToSend = 0;
	unsigned bytesToSend;
	
	// envoyer les données par fragement jusqu'au dernier octet
	char *fragment = new char[MaxMailSize];
	while (bytesTriedToSend < size) {
		bytesToSend = (size - bytesTriedToSend > MaxMailSize? MaxMailSize : size - bytesTriedToSend);
		
		memcpy(fragment, data + bytesSent, bytesToSend);
		mailHdr->length = bytesToSend;
		if (this->SendFiable(*pktHdr, *mailHdr, static_cast<char const *>(fragment))) {
			bytesSent += bytesToSend;
		}
		bytesTriedToSend += bytesToSend;
	}
	
	delete [] fragment;
	delete mailHdr;
	delete pktHdr;
	
	return bytesSent;
}

//----------------------------------------------------------------------
// PostOffice::ReceiveUnfixedSize
//	Reçoit des données d'une taille donnée par l'utilisateur sur un 
//	  certain port local de la machine et les mets dans "data"
//
// "localPort" -- port local de la machine sur lequel seront reçus les 
//					données
// "data" --  pointeur vers une structure où mettre les données reçus
// "size" -- taille des données attendues.
//----------------------------------------------------------------------

	void
PostOffice::ReceiveUnfixedSize(int localPort, char* data, unsigned size)
{
	ASSERT(size <= MaxStringSize);
	
	DEBUG('n', "Asking to Receive message of size %d on box %d\n", size, localPort);
	
	ASSERT((localPort >= 0) && (localPort < numBoxes));
	
	unsigned bytesRemaining = size;
	unsigned bytesToReceive;
	
	PacketHeader *pktHdr = new PacketHeader();
	MailHeader *mailHdr = new MailHeader();
	char *fragment = new char[MaxMailSize];
	
	// recevoir les données par fragments jusqu'au dernier octet
	while (bytesRemaining > 0) {
		bytesToReceive = (bytesRemaining > MaxMailSize? MaxMailSize : bytesRemaining);

		mailHdr->length = bytesToReceive;
		this->Receive(localPort, pktHdr, mailHdr, fragment);
		
		memcpy(data + size - bytesRemaining, fragment, bytesToReceive);
		bytesRemaining -= bytesToReceive;
		
	}
	
	delete [] fragment;
	delete mailHdr;
	delete pktHdr;

}
