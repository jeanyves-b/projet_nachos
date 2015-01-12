#include "syscall.h"

void thread(void*);
void test(void*);

/*
 * Ce programme de test nous permet simplement de tester le nombre de Threads que l'on peut executer
 */

int main(){
	int n = 3;
	int n2 = 5;
	int n3 = 7;
	int error = 0;
	unsigned id = UserThreadCreate(thread, (void*)(&n));
	unsigned id2 = UserThreadCreate(thread, (void*)(&n2));
	unsigned id3 = UserThreadCreate(thread, (void*)(&n3));
	UserThreadCreate(test, (void*)(&n3)); 

	if (id >= 0){
		//error = UserThreadJoin(id); 
		if (error < 0){
		  SynchPutString("erreur Thread 1");
		}
	}
	if (id3 >= 0){
		//error=UserThreadJoin(id3);
		if (error < 0){
		  SynchPutString("erreur Thread 2");
		}
	}
	if (id2 >= 0){
		//error = UserThreadJoin(id2);
		if (error < 0){
		  SynchPutString("erreur Thread 3");
		}
	}

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

void test(void *n){
  SynchPutString("thread test");
  UserThreadExit();
}
