#include "copyright.h"
#include "system.h"
#include "userprocess.h"


void StartUserProcess(int data) {
	currentThread->space = (AddrSpace*)data;
	currentThread->space->InitRegisters ();	// set the initial register values
	currentThread->space->RestoreState ();		// load page table register
	machine->Run();
}

int do_UserProcessCreate(char *s){
	OpenFile *executable = fileSystem->Open (s);
	
	if (executable == NULL)
	{
		printf ("Unable to open file\n");
		return -1;
	}
	
	Thread *newThread = new Thread("user process");
	
	// Si le thread créé est null, on renvoie -1
	if(newThread == NULL)
		return -2;
	 
	newThread->id = 0;
	AddrSpace *addrspace = new AddrSpace (executable);
	
	if(addrspace == NULL)
		return -3;
	
	delete executable;		// close file
	
	newThread->Fork(StartUserProcess, (int)addrspace);

	return 0;
}

void do_UserProcessExit(){
  
}
