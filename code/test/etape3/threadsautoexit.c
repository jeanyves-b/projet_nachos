#include "syscall.h"

void thread(void*);

/*
 * Programme de test de la terminaison automatique
 */

int main(){
	int n = 3;
	int error = 0;
	unsigned id = UserThreadCreate(thread, (void*)(&n));

	if (id >= 0){
		error = UserThreadJoin(id); 
		if (error < 0){
			PutString("erreur Thread 1");
		}
	}

	PutString("On quitte main");
	return 0;
}

/*
 * Affiche des entiers de 0 à *n
 */

void thread(void *n){
	int i=0;

	for(i=0; i <  *((int*)n)  ; i++){	
		PutChar('1');
	}

	PutChar('\n');
	PutString("On quitte un thread \"fils\"");
	//UserThreadExit();
}

