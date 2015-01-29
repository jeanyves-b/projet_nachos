#include <syscall.h>

#define N 500

void thread(void *n)
{
	int i=0;

	for(i=0; i <  *((int*)n)  ; i++)
	{
		PutChar('H');
	}

	PutChar('\n');
	PutString("On quitte un thread créé");
	UserThreadExit();
}

void joinerror3(void *n)
{
		int error = UserThreadJoin(GetTid());
		if (error == -3)
			PutString("On essaie de se Join soi-même et on a le bon code d'erreur.");
		else
			PutString("On essaie de se Join soi-même et on n'a pas le bon code d'erreur.");
	UserThreadExit();
}

int main()
{
	
	int error = 0;
	unsigned id = UserThreadCreate(joinerror3, 0);

	 if (id >= 0)
	{
		error = UserThreadJoin(-1); 
		if (error == -1 || error == -2)
			PutString("On essaie de Join un thrad avec id négatif, et on a le bon code d'erreur.");
		else
			PutString("On essaie de Join un thrad avec id négatif, et on n'a pas le bon code d'erreur.");
	} 

	if (id >= 0)
	{
		error=UserThreadJoin(25);
		if (error == -1 || error == -2)
			PutString("On essaie de Join un thrad avec id jamais créé, et on a le bon code d'erreur.");
		else
			PutString("On essaie de Join un thrad avec id jamais créé, et on n'a pas le bon code d'erreur.");
	} 

	unsigned i;
	int n = 20;
	unsigned ids[N];
	for(i = 0; i<N; i++)
	{
		ids[i] = UserThreadCreate(thread, (void *)&n);
		if (ids[i] == -2 || ids[i] == -1)
		{
			PutString("On a essayé de créer trop de threads, on a le bon code d'erreur.");
			break;
		}
	} 

	PutString("On quitte main");
	return 0;
}
