#include "syscall.h"

void thread(void*);

/*
 * Ce programme de test nous permet simplement de tester le nombre de Threads que l'on peut executer
 */

int main(){
	int n = 10;
	
	// Si le thread est bien lancé, un message positif sera affiché à l'ecran
	UserThreadCreate(thread, (void*)(&n));
	//(UserThreadCreate(thread, (void*)(&n)) != -1)?SynchPutString("\nLe Thread 1 a bien été lancé."):SynchPutString("\nEchec (Thread 1)");

	return 0;
}

/*
 * Affiche des entiers de 0 à n
 */

void thread(void *n){
	volatile int i=0;
	
	PutChar('\n');

	for(i=0; i < *((int*)n); i++){
		SynchPutInt(i);
	}

	PutChar('\n');
}
