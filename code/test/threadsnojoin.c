#include "syscall.h"

void test(void*);
void thread(void*);

/*
 * Ce programme de test nous permet simplement de tester l'execution de plusieurs threads (sans Join)
 */

int main(){
	int n = 3;
	int n2 = 5;
	int n3 = 7;

	UserThreadCreate(thread, (void*)(&n));
	UserThreadCreate(thread, (void*)(&n2));
	UserThreadCreate(thread, (void*)(&n3));
	UserThreadCreate(test, (void*)(&n3)); 

	SynchPutString("On quitte main");
	return 0;
}

/*
 * Affiche des entiers selon le paramètre passé
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

void test(void *n){
	SynchPutString("thread test");
	UserThreadExit();
}
