#include "syscall.h"

void thread(void*);

/*
 * Ce programme de test nous permet simplement de tester le nombre de Threads que l'on peut executer
 */

int main(){
	int n = 3;
	int n2 = 5;
	int n3 = 7;

	unsigned id = UserThreadCreate(thread, (void*)(&n));
	unsigned id2 = UserThreadCreate(thread, (void*)(&n2));
	unsigned id3 = UserThreadCreate(thread, (void*)(&n3));  

	if (id >= 0)
		UserThreadJoin(id); 
	if (id3 >= 0)
		UserThreadJoin(id3);
	if (id2 >= 0)
		UserThreadJoin(id2);

	SynchPutString("On quitte main");
	return 0;
}

/*
 * Affiche des entiers de 0 à *n
 */

void thread(void *n){
	int i=0;



	for(i=0; i < *((int*)n); i++){	
		if (*(int*)n==3)		
			PutChar('a'+i);
		else if (*(int*)n==5)
			PutChar('z'-i);
		else
			PutChar('k'+i);
	}

	PutChar('\n');
	SynchPutString("On quitte un thread \"fils\"");
	UserThreadExit();
}
