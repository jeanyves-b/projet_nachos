#include "syscall.h"

/*
 *  Programme de test userpages0
 */
 
 void thread1(void *n){
	 int i=0;
	 while (i<10) {
		PutChar('a'+i);
		i++;
	}
}

 void thread2(void *n){
	 int i=0;
	 while (i<10) {
		PutChar('0'+i);
		i++;
	}
}

int main()
{
	SynchPutString("zest");
	unsigned id = UserThreadCreate(thread1, 0);
	unsigned id2 = UserThreadCreate(thread2, 0);
	int error;
	
	if (id >= 0){
		error = UserThreadJoin(id); 
		if (error < 0){
			SynchPutString("erreur Thread 1");
		}
	}
	if (id2 >= 0){
		error = UserThreadJoin(id2);
		if (error < 0){
			SynchPutString("erreur Thread 2");
		}
	} 
	PutChar('\n');
	return 0;

}
