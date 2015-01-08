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
	void (*function)(void*);
	void *arg;
};

void StartUserThread(int data) {	
	//FunctionData *function_data = (FunctionData*)data;
	currentThread->space->InitRegisters();
	currentThread->space->RestoreState();
	machine->Run();
}

int UserThreadCreate(void f(void*), void *arg) {
	
	// On bloque les interruptions pour rendre ce bout de code atomique
	IntStatus oldLevel = interrupt->SetLevel (IntOff);

	Thread *newThread = new Thread("new thread");
	
	// Si le thread crée est nul, on renvoie -1
	if(newThread == NULL){
		return -1;
	}
	
	//todo: delete
	FunctionData *data = new FunctionData();
	data->function = f;
	data->arg = arg;
	
	newThread->Fork(StartUserThread, (int)data);
	
	// On réactive les interruptions
    interrupt->SetLevel (oldLevel);
   
	return 0;
}


void do_UserThreadExit(){
  delete currentThread->space;
  currentThread->finish();
  /*NOT REACHED*/
}

