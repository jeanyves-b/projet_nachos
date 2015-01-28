#include "copyright.h"
#include "system.h"
#include "userprocess.h"

//---------------------------------------------------------------------
// StartUserProcess
//	Permet d'inistaliser les registres du processus créé et de mettre 
//		en place les informations sur la table des pages.
//
//---------------------------------------------------------------------

void StartUserProcess(int data) {
	currentThread->space->InitRegisters ();	// set the initial register values
	currentThread->space->RestoreState ();		// load page table register
	machine->Run();
}

//---------------------------------------------------------------------
// do_UserProcessCreate
//	Permet de forker un processus qui lance le fichier executable donné
//		dans s. 
//
//	Retourne le pid du processus en cas de succès, un code d'erreur
//		négatif sinon.
//---------------------------------------------------------------------
int do_UserProcessCreate(char *s){
	printf("here");
	OpenFile *executable = fileSystem->Open(s);
	if (executable == NULL)
		return -1;
	
	AddrSpace *addrSpace = new AddrSpace(executable);

	delete executable;
	if(addrSpace == NULL)
		return -2;

	Thread *newThread = new Thread(s);
	if (newThread == NULL){
		delete addrSpace;
		return -3;
	}
	

	newThread->space = addrSpace;
	newThread->id = 0;
	addrSpace->pid = machine->IncrProcess();
	newThread->ForkExec(StartUserProcess, 0);

	return addrSpace->pid;
}

void do_UserProcessExit(){
	if (currentThread->space != NULL){
		currentThread->space->JoinThreads();
	}
	if(machine->DecrProcess() == 0){
		DEBUG('r', "Exiting program with return value %d.\n",machine->ReadRegister(8));
		interrupt->Halt();
	} else {
		DEBUG('r', "Exiting process %d with return value %d.\n",currentThread->getPid(),machine->ReadRegister(8));
		currentThread->Finish();
	}
	ASSERT(FALSE);
}
