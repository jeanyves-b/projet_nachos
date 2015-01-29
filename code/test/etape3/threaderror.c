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
			PutString("test 3 : OK\n");
		else
			PutString("test 3 : NON\n");
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
			PutString("test 1 : OK\n");
		else
			PutString("test 1 : NON\n");
	} 

	if (id >= 0)
	{
		error=UserThreadJoin(25);
		if (error == -1 || error == -2)
			PutString("test 2 : OK\n");
		else
			PutString("test 2 : NON\n");
	} 

	unsigned i;
	int n = 100;
	unsigned ids[N];
	for(i = 0; i<N; i++)
	{
		ids[i] = UserThreadCreate(thread, (void *)&n);
		if (ids[i] == -2 || ids[i] == -1)
		{
			PutString("test 4 : OK\n");
			break;
		}
	} 

	PutString("On quitte main\n");
	return 0;
}
