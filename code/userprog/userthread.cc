#include "copyright.h"
#include "system.h"
#include "userthread.h"
//---------------------------------------------------------------------
// UserThreadCreate
//	Permet de créer un thread exécutant la fonction f avec 
//	l'argument arg
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// StartUserThread
//	Fonction qui permet de mettre en place les registres (et la pile)
//		d'un thread, les données sont transimses dans "data" sous forme
//		d'un pointeur vers une structure FunctionData. 
//	
//---------------------------------------------------------------------

void StartUserThread(int data) {

	FunctionData *function_data = (FunctionData*)data;

	//Initialisation de tous les registres
	for (int i = 0; i < NumTotalRegs; i++) {
		machine->WriteRegister (i, 0);
	}

	//	mettre le PC à la fonction qu'on veut éxecuter
	machine->WriteRegister(PCReg, function_data->function);
	//	écriture de l'argument dans le registre 4
	machine->WriteRegister(4, function_data->arg);
	//	mettre à jour le NextPC avec l'instruction suivant f
	machine->WriteRegister(NextPCReg, function_data->function + 4);
	//	mettre la valeur de retour à l'adresse de UserThreadExit
	machine->WriteRegister(RetAddrReg, function_data->exit);

	//	mettre le registre de pile au bon endroit
	unsigned stack;
	int err = currentThread->space->GetStackAddress(&stack, function_data->id);
	ASSERT(err >= 0);

	machine->WriteRegister(StackReg, stack);
	delete function_data; //	éviter leak de mémoire

	currentThread->space->RestoreState();
	machine->Run();
}

//---------------------------------------------------------------------
// do_UserThreadCreate
//	Permet de créer un thread exécutant la fonction f avec 
//	l'argument arg et mets exit dans le registre de retour.
//	
//	Retourne l'id unique du thread créé (dans le processus), un nombre 
//		négatif en cas d'erreur
//---------------------------------------------------------------------

int do_UserThreadCreate(int f, int arg, int exit){

	Thread *newThread = new Thread("user thread");

	// Si le thread créé est null, on renvoie -1
	if(newThread == NULL)
		return -1;

	//todo: delete
	FunctionData *data = new FunctionData();
	data->function = f;
	data->arg = arg;
	data->exit = exit;
	data->id = currentThread->AddThread();
	
	if (data->id < 0){
		return -2;
	}
	
	newThread->id = data->id;
	newThread->Fork(StartUserThread, (int)data);

	return newThread->id;
}

//---------------------------------------------------------------------
// do_UserThreadExit
//	Permet de quitter le thread en cours en ayant attendu ses "fils", 
//		et ayant remis sur la file d'attente les threads qui
//		l'attendaient
//	
//---------------------------------------------------------------------
void do_UserThreadExit(){
	currentThread->JoinFils();
	currentThread->space->RunWaitingThread(currentThread->id);
	currentThread->Finish();
}

