#ifndef USERTHREAD_H
#define USERTHREAD_H
#include "copyright.h"
#include "utility.h"
#include "console.h"

//Création de thread utilisateur avec la fonction f et l'argument arg
extern int do_UserThreadCreate(int f, int arg); 
void do_UserThreadExit();

//Structure pour stocker les paramètres d'une fonction
struct FunctionData; 

#endif // USERTHREAD_H
