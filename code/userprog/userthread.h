#ifndef USERTHREAD_H
#define USERTHREAD_H
#include "copyright.h"
#include "utility.h"
#include "console.h"

//Création de thread utilisateur avec la fonction f et l'argument arg
extern int UserThreadCreate(void f(void *a), void* arg); 

#endif // USERTHREAD_H
