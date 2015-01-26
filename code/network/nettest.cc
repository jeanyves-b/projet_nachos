// nettest.cc 
//	Test out message delivery between two "Nachos" machines,
//	using the Post Office to coordinate delivery.
//
//	Two caveats:
//	  1. Two copies of Nachos must be running, with machine ID's 0 and 1:
//		./nachos -m 0 -o 1 &
//		./nachos -m 1 -o 0 &
//
//	  2. You need an implementation of condition variables,
//	     which is *not* provided as part of the baseline threads 
//	     implementation.  The Post Office won't work without
//	     a correct implementation of condition variables.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "system.h"
#include "network.h"
#include "post.h"
#include "interrupt.h"

// Test out message delivery, by doing the following:
//	1. send a message to the machine with ID "farAddr", at mail box #0
//	2. wait for the other machine's message to arrive (in our mailbox #0)
//	3. send an acknowledgment for the other machine's message
//	4. wait for an acknowledgement from the other machine to our 
//	    original message

	void
MailTest(int farAddr)
{
	PacketHeader outPktHdr, inPktHdr;
	MailHeader outMailHdr, inMailHdr;
	const char *data = "Hello there!";
	const char *ack = "Got it!";
	char buffer[MaxMailSize];

	// construct packet, mail header for original message
	// To: destination machine, mailbox 0
	// From: our machine, reply to: mailbox 1
	outPktHdr.to = farAddr;		
	outMailHdr.to = 0;
	outMailHdr.from = 1;
	outMailHdr.length = strlen(data) + 1;

	// Send the first message
	postOffice->Send(outPktHdr, outMailHdr, data); 

	// Wait for the first message from the other machine
	postOffice->Receive(0, &inPktHdr, &inMailHdr, buffer);
	printf("Got \"%s\" from %d, box %d\n",buffer,inPktHdr.from,inMailHdr.from);
	fflush(stdout);

	// Send acknowledgement to the other machine (using "reply to" mailbox
	// in the message that just arrived
	outPktHdr.to = inPktHdr.from;
	outMailHdr.to = inMailHdr.from;
	outMailHdr.length = strlen(ack) + 1;
	postOffice->Send(outPktHdr, outMailHdr, ack); 

	// Wait for the ack from the other machine to the first message we sent.
	postOffice->Receive(1, &inPktHdr, &inMailHdr, buffer);
	printf("Got \"%s\" from %d, box %d\n",buffer,inPktHdr.from,inMailHdr.from);
	fflush(stdout);

	// Then we're done!
	interrupt->Halt();
}

// Teste l'envoi de gros message (900 octets) so us forme de token que la
//	première machine (celle ayant l'id 0) enverra d'abord
//	puis attendra, alors que les autres machines (id supérieur à 0)
//	attendront de recevoir le message avant de l'envoyer. La dernière 
//	machine renverra vers 0.
// Ce test est utile pour tester un anneau.
// option -m <machine id> -ri <taille de l'anneau>
	void
RingMailTest(int size)
{
	char *data = new char[900];
	char *buffer = new char[900];
	
	unsigned i;
	for(i = 0; i < 895; i++) {
		data[i] = netAddr == 0? 'a' + (i%26) : 'z' - (i%26);
	}
	data[i++] = '#';
	data[i++] = 'E';
	data[i++] = 'N';
	data[i++] = 'D';
	data[i] = '\0';

	// To: destination machine, mailbox 0
	// From: our machine, reply to: mailbox 1
	if (netAddr==0)
		postOffice->SendUnfixedSize(data, 900, 1, 1, 0); 

	// Wait for the message from the other machine
	postOffice->ReceiveUnfixedSize(0, buffer, 900);
	printf("Got \"%s\"\n",buffer);
	fflush(stdout);
	
	if (farAddr!=0)
		postOffice->SendUnfixedSize(data, 900, 1, netAddr+1==size?0:netAddr + 1, 0); 

	// Then we're done!
	interrupt->Halt();
}

// Envoie un fichier vers la machine indiquée 
//	option -m <adresse machine> -fs <nom du fichier> <adresse distance>
	void
FileSendTest(const char* file, int farAddr)
{
	postOffice->SendFile(file, 1, farAddr, 0);
	
	// Then we're done!
	interrupt->Halt();
}

// Reçoit un fichier et le mets à l'endroit indiqué
//	option -m <adresse machine> -fr <nom du fichier>
	void
FileReceiveTest(const char* to)
{
	postOffice->ReceiveFile(0, to);
	
	// Then we're done!
	interrupt->Halt();
}
