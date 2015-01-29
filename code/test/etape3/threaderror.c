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
	PutString("On quitte un thread \"fils\"\n");
	UserThreadExit();
}

void joinerror3(void *n)
{
		int error = UserThreadJoin(GetTid());
		if (error == -3)
			PutString("Erreur essaie de se join lui-même : OK\n");
		else
			PutString("Erreur essaie de se join lui-même : NON\n");
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
			PutString("Erreur essaie de join identifiant négatif : OK\n");
		else
			PutString("Erreur essaie de join identifiant négatif : NON\n");
	} 

	if (id >= 0)
	{
		error=UserThreadJoin(25);
		if (error == -1 || error == -2)
			PutString("Erreur essaie de join identifiant jamais créé : OK\n");
		else
			PutString("Erreur essaie de join identifiant jamais créé : NON\n");
	} 

	unsigned i;
	int n = 100;
	unsigned ids[N];
	for(i = 0; i<N; i++)
	{
		ids[i] = UserThreadCreate(thread, (void *)&n);
		if (ids[i] == -2 || ids[i] == -1)
		{
			PutString("Erreur essaie de créer trop de threads : OK\n");
			break;
		}
	} 

	PutString("On quitte main\n");
	return 0;
}
