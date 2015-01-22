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
/* 
	char *data = new char[900];
	char *buffer = new char[900];
	
	unsigned i;
	for(i = 0; i < 895; i++) {
		data[i] = farAddr == 1? 'a' + (i%26) : 'z' - (i%26);
	}
	data[i++] = '#';
	data[i++] = 'E';
	data[i++] = 'N';
	data[i++] = 'D';
	data[i] = '\0';

	// To: destination machine, mailbox 0
	// From: our machine, reply to: mailbox 1
	if (farAddr==1)
	// Send the first message
		postOffice->SendUnfixedSize(data, 900, 1, farAddr, 0); 

	// Wait for the first message from the other machine
	postOffice->ReceiveUnfixedSize(0, buffer, 900);
	printf("Got \"%s\"\n",buffer);
	fflush(stdout);
	
	if (farAddr!=1)
	// Send the first message
		postOffice->SendUnfixedSize(data, 900, 1, farAddr, 0); 

*/

	const char* incoming = "00";
	const char* towrite = "00r";
	if (farAddr == 1) {
		postOffice->SendFile(const_cast<char *>(incoming), 1, farAddr, 0);
	}
	else 
		postOffice->ReceiveFile(0, const_cast<char *>(towrite), 99800);
	
	// Then we're done!
	interrupt->Halt();
}
