#include "copyright.h"
#include "system.h"
#include "userprocess.h"


void StartUserProcess(int data) {
  synchconsole->SynchPutString("test");
	currentThread->space->InitRegisters ();	// set the initial register values
	currentThread->space->RestoreState ();		// load page table register
	machine->Run();
}

int do_UserProcessCreate(char *s){
	Thread *newThread = new Thread("user process");
	synchconsole->SynchPutString(s);
	OpenFile *executable = fileSystem->Open (s);
	AddrSpace *space;
	
	// Si le thread créé est null, on renvoie -1
	if(newThread == NULL)
		return -1;
	

	if (executable == NULL)
	{
		synchconsole->SynchPutString ("Unable to open file");
		return -2;
	}
	space = new AddrSpace (executable);
	 
	newThread->id = 0;
	
	newThread->space = space;
	
	delete executable;		// close file
	
	newThread->Fork(StartUserProcess, 0);

	return 0;
}

void do_UserProcessExit(){
  
}