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
	int id;
};

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
    
    //	mettre le registre de pile au bon endroit
     machine->WriteRegister(StackReg, currentThread->space->GetStackAddress(function_data->id));
	
	//currentThread->space->InitRegisters();
	currentThread->space->RestoreState();
	machine->Run();
}

int do_UserThreadCreate(int f, int arg){
	// On bloque les interruptions pour rendre ce bout de code atomique
	IntStatus oldLevel = interrupt->SetLevel (IntOff);

	Thread *newThread = new Thread("new thread");
	
	// Si le thread crée est nul, on renvoie -1
	if(newThread == NULL)
		return -1;
	
	//todo: delete
	FunctionData *data = new FunctionData();
	data->function = f;
	data->arg = arg;
	data->id = currentThread->space->AddThread();
	
	if (data->id == -1)
		return -1;
		
	newThread->id = data->id;
	newThread->Fork(StartUserThread, (int)data);
	
	// On réactive les interruptions
    interrupt->SetLevel (oldLevel);
   
	return 0;
}

void do_UserThreadExit(){
  currentThread->space->RemoveThread(currentThread->id);
  currentThread->Finish();
  /*NOT REACHED*/
}

