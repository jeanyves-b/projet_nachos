#include "copyright.h"
#include "system.h"
#include "userprocess.h"

void StartUserProcess(int data) {
	currentThread->space->InitRegisters ();	// set the initial register values
	currentThread->space->RestoreState ();		// load page table register

	machine->Run();
}

int do_UserProcessCreate(char *s){

	OpenFile *executable = fileSystem->Open(s);
	if (executable == NULL)
		return -1;
	
	AddrSpace *addrSpace = new AddrSpace(executable);
	
	delete executable;
	if(addrSpace == NULL)
		return -2;

   	Thread *newThread = new Thread(s);
   	if (newThread == NULL)
		return -3;
		
	newThread->space = addrSpace;
	newThread->id = 0;
	
	machine->processCount++;
	
	newThread->ForkExec(StartUserProcess, 0);
	currentThread->Yield();
	return 0;
}

void do_UserProcessExit(){
	machine->processCount--; 
	currentThread->JoinFils();
	delete currentThread->space;
	currentThread->Finish();
	
}
