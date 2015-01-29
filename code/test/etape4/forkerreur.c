#include "syscall.h"

// Programme de tester qui lance deux processus en fork

#define N 1000

int main()
{
	int i, pid;
	pid = ForkExec("papillon");
	if (pid == -1)
		PutString("On essaie de lancer un processus à partir d'un programme inexistant, et on récupère la bonne erreur.\n");
	else
		PutString("On essaie de lancer un processus à partir d'un programme inexistant, et on ne récupère pas la bonne erreur.\n");

	PutString("On essaie de lancer un grand nombre de processus à partir d'un programme gourmand en mémoire jusqu'à ce que ce ne soit plus possible'.\n");
	for (i=0 ; i<N ; i++)
	{
		pid = ForkExec("nothing");
		if(pid == -2 || pid == -3)
		{
			PutString("Impossible de créer le processus, on arrête de crée des processus.");
			i = N;
		}
			
	}
	return 0;
}
