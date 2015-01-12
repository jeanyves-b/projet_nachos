#ifndef USERPROCESS_H
#define USERPROCESS_H
#include "copyright.h"
#include "utility.h"

//Création de thread utilisateur avec la fonction f et l'argument arg
extern int do_UserProcessCreate(char *s); 
void do_UserProcessExit();


#endif // USERPROCESS_H