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

	OpenFile *executable = fileSystem->Open(s);
	if (executable == NULL)
		return -1;
	
	AddrSpace *addrSpace = new AddrSpace(executable);
	
	delete executable;
	if(addrSpace == NULL)
		return -2;
	
	if (addrSpace->HasFailed()) {
		delete addrSpace;
		return -3;
	}

   	Thread *newThread = new Thread(s);
   	if (newThread == NULL)
		return -4;
		
	newThread->space = addrSpace;
	newThread->id = 0;
	
	unsigned pid = machine->IncrProcess();
	
	newThread->ForkExec(StartUserProcess, 0);
	
	return pid;
}

//---------------------------------------------------------------------
// do_UserProcessExit
//	Permet de quitter le processus en cours, en mettant à jour le
//		nombre de processus total, en attendant les threads "fils"
//		de ce processus.
//
//---------------------------------------------------------------------
void do_UserProcessExit(){
	machine->DecrProcess(); 
	currentThread->JoinFils();
	delete currentThread->space;
	currentThread->Finish();
}
