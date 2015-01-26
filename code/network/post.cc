// post.cc 
// 	Routines to deliver incoming network messages to the correct
//	"address" -- a mailbox, or a holding area for incoming messages.
//	This module operates just like the US postal service (in other
//	words, it works, but it's slow, and you can't really be sure if
//	your mail really got through!).
//
//	Note that once we prepend the MailHdr to the outgoing message data,
//	the combination (MailHdr plus data) looks like "data" to the Network 
//	device.
//
// 	The implementation synchronizes incoming messages with threads
//	waiting for those messages.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "post.h"

#include <strings.h> /* for bzero */

//----------------------------------------------------------------------
// Mail::Mail
//      Initialize a single mail message, by concatenating the headers to
//	the data.
//
//	"pktH" -- source, destination machine ID's
//	"mailH" -- source, destination mailbox ID's
//	"data" -- payload data
//----------------------------------------------------------------------

Mail::Mail(PacketHeader pktH, MailHeader mailH, const char *msgData)
{
	ASSERT(mailH.length <= MaxMailSize);

	pktHdr = pktH;
	mailHdr = mailH;
	bcopy(msgData, data, mailHdr.length);
}

//----------------------------------------------------------------------
// MailBox::MailBox
//      Initialize a single mail box within the post office, so that it
//	can receive incoming messages.
//
//	Just initialize a list of messages, representing the mailbox.
//----------------------------------------------------------------------


MailBox::MailBox()
{ 
	messages = new SynchList(); 
}

//----------------------------------------------------------------------
// MailBox::~MailBox
//      De-allocate a single mail box within the post office.
//
//	Just delete the mailbox, and throw away all the queued messages 
//	in the mailbox.
//----------------------------------------------------------------------

MailBox::~MailBox()
{ 
	delete messages; 
}

//----------------------------------------------------------------------
// PrintHeader
// 	Print the message header -- the destination machine ID and mailbox
//	#, source machine ID and mailbox #, and message length.
//
//	"pktHdr" -- source, destination machine ID's
//	"mailHdr" -- source, destination mailbox ID's
//----------------------------------------------------------------------

	void 
PrintHeader(PacketHeader pktHdr, MailHeader mailHdr)
{
	printf("From (%d, %d) to (%d, %d) bytes %d, id %d (%s)\n",
			pktHdr.from, mailHdr.from, pktHdr.to, mailHdr.to, mailHdr.length, mailHdr.id, (mailHdr.type==ACK?"ACK":"MSG"));
}

//----------------------------------------------------------------------
// MailBox::Put
// 	Add a message to the mailbox.  If anyone is waiting for message
//	arrival, wake them up!
//
//	We need to reconstruct the Mail message (by concatenating the headers
//	to the data), to simplify queueing the message on the SynchList.
//
//	"pktHdr" -- source, destination machine ID's
//	"mailHdr" -- source, destination mailbox ID's
//	"data" -- payload message data
//----------------------------------------------------------------------

	void 
MailBox::Put(PacketHeader pktHdr, MailHeader mailHdr, char *data)
{ 
	Mail *mail = new Mail(pktHdr, mailHdr, data); 

	messages->Append((void *)mail);	// put on the end of the list of 
	// arrived messages, and wake up 
	// any waiters
}

//----------------------------------------------------------------------
// MailBox::Get
// 	Get a message from a mailbox, parsing it into the packet header,
//	mailbox header, and data. 
//
//	The calling thread waits if there are no messages in the mailbox.
//
//	"pktHdr" -- address to put: source, destination machine ID's
//	"mailHdr" -- address to put: source, destination mailbox ID's
//	"data" -- address to put: payload message data
//----------------------------------------------------------------------

	void 
MailBox::Get(PacketHeader *pktHdr, MailHeader *mailHdr, char *data) 
{ 
	DEBUG('n', "Waiting for mail in mailbox\n");
	Mail *mail = (Mail *) messages->Remove();	// remove message from list;
	// will wait if list is empty

	*pktHdr = mail->pktHdr;
	*mailHdr = mail->mailHdr;
	if (DebugIsEnabled('n')) {
		printf("Got mail from mailbox: ");
		PrintHeader(*pktHdr, *mailHdr);
	}
	bcopy(mail->data, data, mail->mailHdr.length);
	// copy the message data into
	// the caller's buffer
	delete mail;			// we've copied out the stuff we
	// need, we can now discard the message
}

//----------------------------------------------------------------------
// PostalHelper, ReadAvail, WriteDone
// 	Dummy functions because C++ can't indirectly invoke member functions
//	The first is forked as part of the "postal worker thread; the second
//	as part of the "postal resender" thread; and the
//	later two are called by the network interrupt handler.
//
//	"arg" -- pointer to the Post Office managing the Network
//----------------------------------------------------------------------

static void PostalHelper(int arg)
{ PostOffice* po = (PostOffice *) arg; po->PostalDelivery(); }
static void PostalResender(int arg)
{ PostOffice* po = (PostOffice *) arg; po->PostalSender(); }
static void ReadAvail(int arg)
{ PostOffice* po = (PostOffice *) arg; po->IncomingPacket(); }
static void WriteDone(int arg)
{ PostOffice* po = (PostOffice *) arg; po->PacketSent(); }

//----------------------------------------------------------------------
// PostOffice::PostOffice
// 	Initialize a post office as a collection of mailboxes.
//	Also initialize the network device, to allow post offices
//	on different machines to deliver messages to one another.
//
//      We use a separate thread "the postal worker" to wait for messages 
//	to arrive, and deliver them to the correct mailbox.  Note that
//	delivering messages to the mailboxes can't be done directly
//	by the interrupt handlers, because it requires a Lock.
//
//	"addr" is this machine's network ID 
//	"reliability" is the probability that a network packet will
//	  be delivered (e.g., reliability = 1 means the network never
//	  drops any packets; reliability = 0 means the network never
//	  delivers any packets)
//	"nBoxes" is the number of mail boxes in this Post Office
//----------------------------------------------------------------------

PostOffice::PostOffice(NetworkAddress addr, double reliability, int nBoxes)
{
	// First, initialize the synchronization with the interrupt handlers
	messageAvailable = new Semaphore("message available", 0);
	messageSent = new Semaphore("message sent", 0);
	sendLock = new Lock("message send lock");

	// Second, initialize the mailboxes
	netAddr = addr; 
	numBoxes = nBoxes;
	boxes = new MailBox[nBoxes];
	
	// Troisièmement, initialiser les différentes structures du protocole
	outMsgCount = 0;
	inMsgCount = 0;
	lastAck = -1;
	waitingForAck = NULL;
	hasMessagePending = false;
	isResending = false;
	
	startResendingMsg = new Semaphore("start/block sender", 0);
	checkAck = new Semaphore("start/block worker", 0);
	ackDone = new Semaphore("check for ack done", 0);
	daemonsLock = new Lock("daemons");
	
	// Forth, initialize the network; tell it which interrupt handlers to call
	network = new Network(addr, reliability, ReadAvail, WriteDone, (int) this);


	// Finally, create a thread whose sole job is to wait for incoming messages,
	//   and put them in the right mailbox. 
	Thread *t_worker = new Thread("postal worker");
	Thread *t_sender = new Thread("postal resender");
	t_worker->id = -1;
	t_sender->id = -2;

	t_worker->Fork(PostalHelper, (int) this);
	t_sender->Fork(PostalResender, (int) this);
}

//----------------------------------------------------------------------
// PostOffice::~PostOffice
// 	De-allocate the post office data structures.
//----------------------------------------------------------------------

PostOffice::~PostOffice()
{
	delete network;
	delete [] boxes;
	delete messageAvailable;
	delete messageSent;
	delete sendLock;
}

//----------------------------------------------------------------------
// PostOffice::PostalDelivery
// 	Démon qui tourne en permanence sur le thread "sender" et qui 
//	  renvoie un paquet si celui-ci a été envoyé, n'a toujours pas
//	  reçu d'acquittement, n'a pas atteint son nombre maximal de 
//	  rentransmission, et que sa dernière transmission date de plus de
//	  "TEMPO" secondes.
//
//---------------------------------------
	void
PostOffice::PostalSender() {
	time_t lastTry;			// Le temps de la dernière fois que l'on a tenté d'envoyer le message
	unsigned tryCount ; 		// Le
	for(;;) {
		// attendre une demande de retransmission
		startResendingMsg->P();
		
		// signifier qu'un retransmission est en cours
		daemonsLock->Acquire();
		isResending = true;
		daemonsLock->Release();
		
		// retransmettre tant que le nombre maximal de retransmissions
		//	 n'a pas été atteint et qu'aucun acquittement reçu.
		time(&lastTry);
		tryCount = 1;
		while(tryCount < MAXREEMISSIONS) {
			// passer la main au démon "woker" s'il y a un message
			//	 en attente, et attendre sa permission pour reprendre
			//	 et si à la reprise l'acquittement a bien été reçu, 
			//	 redonner la main au thread principal
			daemonsLock->Acquire();
			if (hasMessagePending) {
				hasMessagePending = false; // signifier que le message
				// en attente a été pris en compte
				daemonsLock->Release();
				
				checkAck->V(); // donner la main au démon "worker"
				startResendingMsg->P(); // attendre de reprendre la main
				
				if (waitingForAck == NULL) {
					daemonsLock->Acquire();
					isResending = false; // signifier qu'il n'y a plus
					// de retransmission en cours
					daemonsLock->Release();
					ackDone->V(); // signifier que la vérification 
					// est terminée
					break;
				}
			} else {
				daemonsLock->Release();
			}
			
			// renvoyer le message s'il est temps de le faire
			if (waitingForAck != NULL && difftime(time(NULL), lastTry) >= TEMPO) {
				DEBUG('n', "Le message #%d n'a pas été acquitté après %.2lf secondes, il est retransmis (essai n°%d).\n", 
					waitingForAck->mailHdr.id, TEMPO, tryCount+1);
				
				char* buffer = new char[MaxPacketSize];	// space to hold concatenated
				// mailHdr + data

				// concatenate MailHeader and data
				bcopy(&(waitingForAck->mailHdr), buffer, sizeof(MailHeader));
				bcopy(waitingForAck->data, buffer + sizeof(MailHeader), waitingForAck->mailHdr.length);
				
				// mettre à jour nombre d'essais et temps dernier essai
				tryCount++;	
				time(&lastTry);

				network->Send(waitingForAck->pktHdr, buffer);
				messageSent->P();			// wait for interrupt to tell us

				delete [] buffer;		
				// this->Send(waitingForAck->pktHdr, waitingForAck->mailHdr, waitingForAck->data);
			}
		} 
		
		// si aucun acquittement de reçu tout au long des retransmissions
		if (tryCount == MAXREEMISSIONS) {
			DEBUG('n', "Le message #%d n'a pas été acquitté après %.2lf secondes, il a été détruit.\n", 
				waitingForAck->mailHdr.id, MAXREEMISSIONS*TEMPO);
					
			// detruire le message en attente d'acquittement
			delete waitingForAck;
			waitingForAck = NULL; 

			daemonsLock->Acquire();
			isResending = false; // signifier qu'aucun retransmission
			// en cours
			daemonsLock->Release();
			ackDone->V(); // redonner la main au thread principal
		} 
	}
}

//----------------------------------------------------------------------
// PostOffice::PostalDelivery
// 	Wait for incoming messages, and put them in the right mailbox.
//
//      Incoming messages have had the PacketHeader stripped off,
//	but the MailHeader is still tacked on the front of the data.
//
//	Renvoie un acquittement pour les messages normaux et signifie qu'un
//	  message a été acquitté si le message reçu est un acquittement.
//----------------------------------------------------------------------

	void
PostOffice::PostalDelivery()
{
	PacketHeader pktHdr;
	MailHeader mailHdr;
	char *buffer = new char[MaxPacketSize];

	for (;;) {		
		// first, wait for a message
		messageAvailable->P();	
		
		// Signifier qu'un message est en attente
		daemonsLock->Acquire();
		hasMessagePending = true;
		daemonsLock->Release();
		
		pktHdr = network->Receive(buffer);

		mailHdr = *(MailHeader *)buffer;
		if (DebugIsEnabled('n')) {
			printf("Putting mail into mailbox: ");
			PrintHeader(pktHdr, mailHdr);
		}
		
		// ne continuer que s'il n'y pas de retransmission en cours
		//	ou que la retransmission nous permet de continuer ici.
		daemonsLock->Acquire();
		if (isResending) {
			daemonsLock->Release();
			checkAck->P();
		} else {
			daemonsLock->Release();
		}
		
		
		if (mailHdr.type == ACK) {
			if (waitingForAck != NULL) {
				if (waitingForAck->mailHdr.id == mailHdr.id) {
					//	si on reçoit l'acquittement on le signifie
					DEBUG('n', "Le message #%d a été acquitté\n", mailHdr.id);
					delete waitingForAck;
					waitingForAck = NULL;
					lastAck = mailHdr.id;
				} else {
					// utilie pour le debug uniquement
					DEBUG('n', "Reçu message acquittement #%d a été acquitté\n", mailHdr.id);
				}
			}	
		} else {
			//	Renvoyer un acquittement si on reçoit un message normal		
			if (DebugIsEnabled('n')) {
				printf("Sending ACK for message: ");
				PrintHeader(pktHdr, mailHdr);
			}
			PacketHeader ackPHdr; 
			MailHeader ackMHdr;
			
			ackPHdr.to = pktHdr.from;
			ackPHdr.from = netAddr;
			ackPHdr.length = 1;
			
			ackMHdr.to = mailHdr.from;
			ackMHdr.from = mailHdr.to;
			ackMHdr.type = ACK;
			ackMHdr.id = mailHdr.id;
			ackMHdr.length = 1;
			
			this->Send(ackPHdr, ackMHdr, "");
				
			if (mailHdr.id < inMsgCount) {
				DEBUG('n', "Message #%d déjà reçu.\n", mailHdr.id);
			} else {
				DEBUG('n', "__Message #%d jamais reçu. (%d)\n", mailHdr.id, inMsgCount);
				inMsgCount++;
				boxes[mailHdr.to].Put(pktHdr, mailHdr, buffer + sizeof(MailHeader));
			}
		} 
		
		// signifier qu'aucun message n'est attente et redonner la main
		//	 au demon "sender" s'il était entrain de retransmettre
		daemonsLock->Acquire();
		hasMessagePending = false;
		if (isResending) {
			daemonsLock->Release();
			startResendingMsg->V();
		} else {
			daemonsLock->Release();
		}
		
	}
}

//----------------------------------------------------------------------
// PostOffice::Send
// 	Concatenate the MailHeader to the front of the data, and pass 
//	the result to the Network for delivery to the destination machine.
//
//	Note that the MailHeader + data looks just like normal payload
//	data to the Network.
//
//	"pktHdr" -- source, destination machine ID's
//	"mailHdr" -- source, destination mailbox ID's
//	"data" -- payload message data
//----------------------------------------------------------------------

	void
PostOffice::Send(PacketHeader pktHdr, MailHeader mailHdr, const char* data)
{
	char* buffer = new char[MaxPacketSize];	// space to hold concatenated
	// mailHdr + data

	if (DebugIsEnabled('n')) {
		printf("Post send: ");
		PrintHeader(pktHdr, mailHdr);
	}
	ASSERT(mailHdr.length <= MaxMailSize);
	ASSERT(0 <= mailHdr.to && mailHdr.to < numBoxes);

	// fill in pktHdr, for the Network layer
	pktHdr.from = netAddr;
	pktHdr.length = mailHdr.length + sizeof(MailHeader);

	// concatenate MailHeader and data
	bcopy(&mailHdr, buffer, sizeof(MailHeader));
	bcopy(data, buffer + sizeof(MailHeader), mailHdr.length);

	sendLock->Acquire();   		// only one message can be sent
	// to the network at any one time
	network->Send(pktHdr, buffer);
	messageSent->P();			// wait for interrupt to tell us
	// ok to send the next message
	sendLock->Release();

	delete [] buffer;			// we've sent the message, so
	// we can delete our buffer
}



//----------------------------------------------------------------------
// PostOffice::SendSafe
// 	Mets l'entête du mail devant les données, et transmets la demande
//	  d'envoi au réseau, et attends l'acquittement du message en
//	  question avant de retourner. 
//	Un seul paquet peut être transmis à la fois, l'envoi de paquets 
//	  via le réseau est donc synchrone
//	Le paquet est retransmis tout les "TEMPO" secondes tant que 
//	  son acquittement n'a pas été reçu ou qu'il ait été renvoyé
//	  "MAXREEMISSIONS" fois. Au retour de cette fonction, le paquet
//	  pourrait donc ne pas avoir bien été reçu par le destinataire.
//
//	Note that the MailHeader + data looks just like normal payload
//	data to the Network.
//
//	Renvoie si oui ou non le paquet a été bien acquitté.
//
//	"pktHdr" -- source, destination machine ID's
//	"mailHdr" -- source, destination mailbox ID's
//	"data" -- payload message data
//----------------------------------------------------------------------

	bool
PostOffice::SendSafe(PacketHeader pktHdr, MailHeader mailHdr, const char* data)
{
	
	if (DebugIsEnabled('n')) {
		printf("Sending with ACK request: ");
		PrintHeader(pktHdr, mailHdr);
	}
	ASSERT(mailHdr.length <= MaxMailSize);
	ASSERT(0 <= mailHdr.to && mailHdr.to < numBoxes);
	
	mailHdr.type = MSG; // message normal
	mailHdr.id = outMsgCount++; // attribution d'un identfiant

	if (DebugIsEnabled('n')) {
		printf("Post send: ");
		PrintHeader(pktHdr, mailHdr);
	}
	
	// fill in pktHdr, for the Network layer
	pktHdr.from = netAddr;
	pktHdr.length = mailHdr.length + sizeof(MailHeader);

	char* buffer = new char[MaxPacketSize];	// space to hold concatenated
	// mailHdr + data

	// concatenate MailHeader and data
	bcopy(&(mailHdr), buffer, sizeof(MailHeader));
	bcopy(data, buffer + sizeof(MailHeader), mailHdr.length);

	sendLock->Acquire();
	waitingForAck = new Mail(pktHdr, mailHdr, data);
	
	network->Send(waitingForAck->pktHdr, buffer);
	messageSent->P();			// wait for interrupt to tell us

	delete [] buffer;			// we've sent the message, so
	// we can delete our buffer
	
	// réveillier le sender
	startResendingMsg->V(); 
	
	ackDone->P(); // attendre que la vérification d'acquittement se
	//  termine
	bool retVal = mailHdr.id == lastAck;
	sendLock->Release();
	
	return retVal;

}

//----------------------------------------------------------------------
// PostOffice::Receive
// 	Retrieve a message from a specific box if one is available, 
//	otherwise wait for a message to arrive in the box.
//
//	Note that the MailHeader + data looks just like normal payload
//	data to the Network.
//
//
//	"box" -- mailbox ID in which to look for message
//	"pktHdr" -- address to put: source, destination machine ID's
//	"mailHdr" -- address to put: source, destination mailbox ID's
//	"data" -- address to put: payload message data
//----------------------------------------------------------------------

	void
PostOffice::Receive(int box, PacketHeader *pktHdr, 
		MailHeader *mailHdr, char* data)
{
	ASSERT((box >= 0) && (box < numBoxes));

	boxes[box].Get(pktHdr, mailHdr, data);
	
	ASSERT(mailHdr->length <= MaxMailSize);
}

//----------------------------------------------------------------------
// PostOffice::IncomingPacket
// 	Interrupt handler, called when a packet arrives from the network.
//
//	Signal the PostalDelivery routine that it is time to get to work!
//----------------------------------------------------------------------

	void
PostOffice::IncomingPacket()
{ 
	messageAvailable->V(); 
}

//----------------------------------------------------------------------
// PostOffice::PacketSent
// 	Interrupt handler, called when the next packet can be put onto the 
//	network.
//
//	The name of this routine is a misnomer; if "reliability < 1",
//	the packet could have been dropped by the network, so it won't get
//	through.
//----------------------------------------------------------------------

	void 
PostOffice::PacketSent()
{ 
	messageSent->V();
}

