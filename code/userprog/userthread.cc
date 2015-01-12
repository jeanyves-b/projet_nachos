#include "copyright.h"
#include "system.h"
#include "userthread.h"
//---------------------------------------------------------------------
// UserThreadCreate
//	Permet de créer un thread exécutant la fonction f avec 
//	l'argument arg
//---------------------------------------------------------------------

typedef struct FunctionData FunctionData;
struct FunctionData {
	int function;
	int arg;
	int exit;
	unsigned id;
};

void StartUserThread(int data) {
	//synchconsole->SynchPutString("here");
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

int do_UserThreadCreate(int f, int arg, int exit){
	// On bloque les interruptions pour rendre ce bout de code atomique
	IntStatus oldLevel = interrupt->SetLevel (IntOff);

	Thread *newThread = new Thread("user thread");

	// Si le thread créé est null, on renvoie -1
	if(newThread == NULL)
		return -1;

	//todo: delete
	FunctionData *data = new FunctionData();
	data->function = f;
	data->arg = arg;
	data->exit = exit;

	if (currentThread->AddThread(&(data->id)) < 0){
	  return -2;
	}
	newThread->id = data->id;
	newThread->Fork(StartUserThread, (int)data);

	// On réactive les interruptions
	(void)interrupt->SetLevel (oldLevel);

	return data->id;
}

void do_UserThreadExit(){
	currentThread->JoinFils();
	currentThread->space->RunWaitingThread(currentThread->id);
	//currentThread->space->RemoveThread(currentThread->id);
	currentThread->Finish();
}

