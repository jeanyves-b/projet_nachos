#ifndef USERTHREAD_H
#define USERTHREAD_H
#include "copyright.h"
#include "utility.h"
#include "console.h"

//Création de thread utilisateur avec la fonction f et l'argument arg
extern int do_UserThreadCreate(int f, int arg, int exit); 
void do_UserThreadExit();

//Structure pour stocker les paramètres d'une fonction
typedef struct FunctionData {
	int function;
	int arg;
	int exit;
	unsigned id;
} FunctionData;

#endif // USERTHREAD_H
