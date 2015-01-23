#include "syscall.h"

/*
 *  Programme de test userpages0
 */

void thread1(void *n){
	int i=0;
	while (i<1) {
		PutChar('a'+i);
		i++;
	}

}

void thread2(void *n){
	int i=0;
	while (i<1) {
		PutChar('0'+i);
		i++;
	}

}

int main()
{
  
	 PutString("In Userpage");
	 PutString("Still working");
// 	int id = UserThreadCreate(thread1, 0);
// 	int id2 = UserThreadCreate(thread2, 0);
// 	int error;
// 
// 	error = UserThreadJoin(id); 
// 	if (error < 0){
// 		PutInt(id);
// 		PutInt(error);
// 	}
// 	//~ }
// 	//~ if (id2 >= 0){
// 	error = UserThreadJoin(id2);
// 	if (error < 0){
// 		PutString("erreur Thread 2");
// 	}
	return 0;

}
