// filepost.cc 
//	Méthodes pour l'envoi et la réception de fichiers par réseau entre
//		deux machines.

#include "copyright.h"
#include "post.h"
#include "system.h"

//----------------------------------------------------------------------
// PostOffice::SendFile
//
//----------------------------------------------------------------------

	int
PostOffice::SendFile(char *path, int localPort, MailBoxAddress to, int remotePort)
{
	ASSERT((localPort >= 0) && (localPort < numBoxes));
	OpenFile *file = fileSystem->Open(path);
	if (file == NULL && file->Length()>0)
		return -1;
	
	char *buffer = new char[file->Length()];

	int remaining = file->Length();
	while (remaining > 0) {
		remaining -= file->Read(buffer + file->Length() - remaining, remaining);
	}
	
	int sent = this->SendUnfixedSize(buffer, (unsigned)(file->Length()), localPort, to, remotePort);
	
	delete [] buffer;
	
	if (sent < file->Length()) 
		return -3;
		
	return 0;
}
	
//----------------------------------------------------------------------
// PostOffice::ReceiveFile
//
//----------------------------------------------------------------------

	int
PostOffice::ReceiveFile(int localPort, char *path, unsigned size) {
		
	ASSERT(size > 0);
	ASSERT((localPort >= 0) && (localPort < numBoxes));
	
	
	int retVal = 0;
	
	if (!fileSystem->Create(path, size)) {
		retVal = -1;
	} else {
		OpenFile *file = fileSystem->Open(path);
		if (file == NULL) {
			retVal = -2;
		} else {
			char *buffer = new char[size];
			this->ReceiveUnfixedSize(localPort, buffer, size);
			
			int written = 0;
			while ((unsigned)written < size) {
				written += file->Write(buffer + written, size  - written);
			}
			delete [] buffer;				
		}
	}
	
	return retVal;
}
