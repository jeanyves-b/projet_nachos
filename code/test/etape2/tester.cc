#include <stdio.h>
#include <string>

#define NBTEST 4

int TestString(string entree, string sortie)
{
	if (entree.compare(sortie))
	{
		printf("..... OK\n");
		return 0;
	}
	else
	{
		printf("..... NON OK\n");
		return 1;
	}
}

int main()
{
	//intitialisation des constante de tests
	int testnb = 0;
	int nbfautes = 0;
	char filein[10] = "in0.txt\0";
	char fileout[10] = "out0.txt\0";
	char *argv[];
	char *envp[] = {NULL};

	for (int i=0; i<NBTEST; i++)
	{

		filein[3] = *itoa(i);
		fileout[4] = *itoa(i);

		arg = {"./../../build/nachos-step2", filein, fileout, NULL};
		execve("./../../build/nachos-step2", argv, envp);
		ifstream in(filein);
		ifstream out(fileout);

		while(!(in.eof || out.eof))
		{
			getline(in, entree);
			getline(out, sortie);

			printf("test N° : %4d ", testnb++);
			TestString(entree, sortie);
			nbfautes += teststring();
		}
	}

	//affichage des résultats de tests
	printf("%d tests done\n%d faults", testnb, nbfautes);
	
}