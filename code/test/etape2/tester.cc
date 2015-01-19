//----------------------------------------------------------------------
//ce programme est utilisé pour tester l'étape 2
//il éxécute ./nachos-step2 avec plusieurs fichiers différent
//puis compare la sortie attendue et la sortie effective
//si les deux diffères, le programme print qu'il y a eu une erreur
//sinon il print que tout va bien
//le programme s'arrète quand un nombre de fichier équivalent à NBTEST
//à été tésté.
//----------------------------------------------------------------------

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
	char fileexpe[10] = "expe0.txt\0";
	char *argv[];
	char *envp[] = {NULL};

	for (int i=0; i<NBTEST; i++)
	{
		filein[3] = *itoa(i);
		fileout[4] = *itoa(i);
		fileexpe[5]
		ifstream in(filein);
		ifstream out(fileout);

		arg = {"./../../build/nachos-step2 -sc", filein, fileout, NULL};
		execve("./../../build/nachos-step2 -sc", argv, envp);

		while(!(in.eof || out.eof))
		{
			getline(expe, entree);
			getline(out, sortie);

			printf("test N° : %4d ", testnb++);
			TestString(entree, sortie);
			nbfautes += teststring();
		}
	}

	//affichage des résultats de tests
	printf("%d tests done\n%d faults", testnb, nbfautes);
}