// addrspace.h 
//      Data structures to keep track of executing user programs 
//      (address spaces).
//
//      For now, we don't keep any information about address spaces.
//      The user level CPU state is saved and restored in the thread
//      executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#include "synch.h"
#include <vector>
#include "bitmap.h"

#define UserStackSize		2048	// increase this as necessary!
#define THREAD_PAGES		2u		// on alloue THREAD_PAGES pages par thread; u pour unsigned
#define MAX_THREADS			1024u		// on alloue THREAD_PAGES pages par thread; u pour unsigned


//Structure pour stocker les threads en attente
typedef struct WaitingThread WaitingThread;
struct WaitingThread; 

class AddrSpace
{
	public:
		AddrSpace (OpenFile * executable);	// Create an address space,
		// initializing it with the program
		// stored in the file "executable"
		~AddrSpace ();		// De-allocate an address space

		void InitRegisters ();	// Initialize user-level CPU registers,
		// before jumping to user code

		void SaveState ();		// Save/restore address space-specific
		void RestoreState ();	// info on a context switch 

		int AddThread(unsigned*);	// ajouter un thread
		int RemoveThread(unsigned);	// enlever un thread
		void RunWaitingThread(unsigned);// lancer les threads en attente
		int GetStackAddress(unsigned*,unsigned);	//récupérer l'adresse d'un thread
		//	dans la pile à partir de son identifiant
		int JoinThread (unsigned); //	attendre la fin d'un thread
		bool HasFailed(); //	si le mapping vpn->ppn a échoué



	private:
		TranslationEntry * pageTable;	// Assume linear page table translation
		// for now!
		unsigned int numPages;	// Number of pages in the virtual 
		// address space
		unsigned *threads_stack_id;	// Tableau de: identifiant unique -> 
		//	identifiant dans la pile (0 et 1 sont reservés pour resp.
		//	jamais créé, et créé mais terminé)
		bool *stack_blocs; //Tableau representant si oui ou non un bloc
		//	de la pile est alloué ou non
		unsigned threads_created; //	Nombre de threads créés depuis
		//	le début du processus
		std::vector<WaitingThread*> waiting_threads; //	structure contnenant
		//	les threads en attente associés aux threads qu'ils attendent

		unsigned assigned_vpn; //	nombre de pages virtuelles assignées
		Lock *addT; //mutex pour l'ajout d'un thread
		Lock *waitT; //mutex pout la manipulation du vecteur

		int GetFirstFreeThreadStackBlockId(unsigned*); //	premier bloc allouable
		// dans la pile de taille THREAD_PAGES


};


#endif // ADDRSPACE_H
