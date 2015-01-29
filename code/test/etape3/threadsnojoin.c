#include "syscall.h"

void test(void*);
void thread(void*);

/*
 * Programme de test de l'execution de plusieurs threads sans join
 */

int main(){
	int n = 3;
	int n2 = 5;
	int n3 = 7;

	UserThreadCreate(thread, (void*)(&n));
	UserThreadCreate(thread, (void*)(&n2));
	UserThreadCreate(thread, (void*)(&n3));
	UserThreadCreate(test, (void*)(&n3)); 

	PutString("On quitte main.");
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
	PutString("On atteint la fin d'un thread lancé.");
}

void test(void *n){
	PutString("On est dans le thread test lancé et on a fini.");
}
