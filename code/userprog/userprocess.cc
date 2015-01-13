#include "copyright.h"
#include "system.h"
#include "userprocess.h"


void StartUserProcess(int data) {
	synchconsole->SynchPutString("here\n");
	currentThread->space->InitRegisters ();	// set the initial register values
	currentThread->space->RestoreState ();		// load page table register
	machine->Run();
}

int do_UserProcessCreate(char *s){
	
	
	Thread *newThread = new Thread("user process");
	int error;
	// Si le thread créé est null, on renvoie -1
	if(newThread == NULL)
		return -1;
	 
	newThread->id = 0;
	
	error = newThread->ForkExec(s);
	 if (error < 0){
	   return error;
	 }
	
	return 0;
}

void do_UserProcessExit(){
  
}
