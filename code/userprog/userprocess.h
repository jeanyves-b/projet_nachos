#ifndef USERPROCESS_H
#define USERPROCESS_H
#include "copyright.h"
#include "utility.h"

extern void StartUserProcess(int);
//Création de thread utilisateur avec la fonction f et l'argument arg
extern int do_UserProcessCreate(char *s); 


#endif // USERPROCESS_H
