// addrspace.cc 
//      Routines to manage address spaces (executing user programs).
//
//      In order to run a user program, you must:
//
//      1. link with the -N -T 0 option 
//      2. run coff2noff to convert the object file to Nachos format
//              (Nachos object code format is essentially just a simpler
//              version of the UNIX executable object code format)
//      3. load the NOFF file into the Nachos file system
//              (if you haven't implemented the file system yet, you
//              don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "frameprovider.h"
#include "addrspace.h"
#include "noff.h"
//~ #include <strings.h>		/* for bzero */

typedef struct WaitingThread {
	Thread *who;
	int forId;
} WaitingThread;

static void
ReadAtVirtual(OpenFile *executable, int virtualaddr, int numBytes,
    int position, TranslationEntry *pageTableA, unsigned numPagesA){
    
    TranslationEntry *oldPT = machine->pageTable;
    unsigned oldPTS = machine->pageTableSize;

    machine->pageTable = pageTableA;
    machine->pageTableSize = numPagesA;

    char buffer[numBytes];


    int written = executable->ReadAt(buffer, numBytes, position);
    int i = 0;

    for (i = 0; i < written; i++){
        machine->WriteMem(virtualaddr+i, 1, buffer[i]);
    }


    machine->pageTable = oldPT;
    machine->pageTableSize = oldPTS;
} 

//----------------------------------------------------------------------
// SwapHeader
//      Do little endian to big endian conversion on the bytes in the 
//      object file header, in case the file was generated on a little
//      endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

	static void
SwapHeader (NoffHeader * noffH)
{
	noffH->noffMagic = WordToHost (noffH->noffMagic);
	noffH->code.size = WordToHost (noffH->code.size);
	noffH->code.virtualAddr = WordToHost (noffH->code.virtualAddr);
	noffH->code.inFileAddr = WordToHost (noffH->code.inFileAddr);
	noffH->initData.size = WordToHost (noffH->initData.size);
	noffH->initData.virtualAddr = WordToHost (noffH->initData.virtualAddr);
	noffH->initData.inFileAddr = WordToHost (noffH->initData.inFileAddr);
	noffH->uninitData.size = WordToHost (noffH->uninitData.size);
	noffH->uninitData.virtualAddr =
		WordToHost (noffH->uninitData.virtualAddr);
	noffH->uninitData.inFileAddr = WordToHost (noffH->uninitData.inFileAddr);
}

  

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
//      Create an address space to run a user program.
//      Load the program from a file "executable", and set everything
//      up so that we can start executing user instructions.
//
//      Assumes that the object code file is in NOFF format.
//
//      First, set up the translation from program memory to physical 
//      memory.  For now, this is really simple (1:1), since we are
//      only uniprogramming, and we have a single unsegmented page table
//
//      "executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace (OpenFile *executable)
{
	ASSERT(UserStackSize>=ThreadPages*(PageSize+16));

	NoffHeader noffH;
	unsigned size;

	executable->ReadAt ((char *) &noffH, sizeof (noffH), 0);
	if ((noffH.noffMagic != NOFFMAGIC) &&
			(WordToHost (noffH.noffMagic) == NOFFMAGIC))
		SwapHeader (&noffH);
	ASSERT (noffH.noffMagic == NOFFMAGIC);

	// how big is address space?
	size = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStackSize;	// we need to increase the size
	// to leave room for the stack
	numPages = divRoundUp (size, PageSize);
	size = numPages * PageSize;

	ASSERT (numPages <= NumPhysPages);	// check we're not trying
	// to run anything too big --
	// at least until we have
	// virtual memory
	
	//	initialisation des variables de gestion des threads de l'espace
	//		d'adressage
	threads_stack_id = new unsigned[MaxThreads];
	stack_blocs = new bool[MaxRunningThreads];
	for(unsigned int j=0; j<MaxThreads; j++)
		threads_stack_id[j] = 0;

	for(unsigned int j=0; j<MaxRunningThreads; j++)
		stack_blocs[j] = false;
		
	threads_created = 0; 

	
	DEBUG ('a', "Initializing address space, num pages %d, size %d\n",
			numPages, size);
	// first, set up the translation 
	
	pageTable = new TranslationEntry[numPages];
		
	for (assigned_vpn = 0; assigned_vpn < numPages; assigned_vpn++)
	{
		pageTable[assigned_vpn].virtualPage = assigned_vpn;	
		if (machine->frameprovider->NumAvailFrame() > 0) { //	on vérifie qu'il y a assez de pages pages 
			pageTable[assigned_vpn].physicalPage = machine->frameprovider->GetEmptyFrame();
		} else {
			return;
		}
		pageTable[assigned_vpn].valid = TRUE;
		pageTable[assigned_vpn].use = FALSE;
		pageTable[assigned_vpn].dirty = FALSE;
		pageTable[assigned_vpn].readOnly = FALSE;	// if the code segment was entirely on 
		// a separate page, we could set its 
		// pages to be read-only
	}

	// zero out the entire address space, to zero the unitialized data segment 
	// and the stack segment
	//bzero (machine->mainMemory, size);
	
	// then, copy in the code and data segments into memory
	if (noffH.code.size > 0)
	{
		DEBUG ('a', "Initializing code segment, at 0x%x, size %d\n",
				noffH.code.virtualAddr, noffH.code.size);
		ReadAtVirtual(executable, noffH.code.virtualAddr, noffH.code.size, 
            noffH.code.inFileAddr, pageTable, numPages);
	}
	if (noffH.initData.size > 0)
	{
		DEBUG ('a', "Initializing data segment, at 0x%x, size %d\n",
				noffH.initData.virtualAddr, noffH.initData.size);
		ReadAtVirtual(executable, noffH.initData.virtualAddr, noffH.initData.size, 
            noffH.initData.inFileAddr, pageTable, numPages); 
		}

	addT = new Lock("add");
	waitT = new Lock("waitT");

	

}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
//      Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace ()
{
	// LB: Missing [] for delete

	// End of modification
	unsigned i;
	//liberation de toutes les pages physiques utilisées par le processus
	for(i = 0; i < assigned_vpn; i++)
		machine->frameprovider->ReleaseFrame(pageTable[i].physicalPage);
		// delete pageTable;
	delete [] pageTable;
	delete stack_blocs;
	delete threads_stack_id;
	waiting_threads.clear();
	delete addT;
	delete waitT;
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
//      Set the initial values for the user-level register set.
//
//      We write these directly into the "machine" registers, so
//      that we can immediately jump to user code.  Note that these
//      will be saved/restored into the currentThread->userRegisters
//      when this thread is context switched out.
//----------------------------------------------------------------------

	void
AddrSpace::InitRegisters ()
{
	int i;

	for (i = 0; i < NumTotalRegs; i++)
		machine->WriteRegister (i, 0);

	// Initial program counter -- must be location of "Start"
	machine->WriteRegister (PCReg, 0);

	// Need to also tell MIPS where next instruction is, because
	// of branch delay possibility
	machine->WriteRegister (NextPCReg, 4);

	// Set the stack register to the end of the address space, where we
	// allocated the stack; but subtract off a bit, to make sure we don't
	// accidentally reference off the end!
	machine->WriteRegister (StackReg, numPages * PageSize - 16);

	//	Ne pas oublier le thread main
	unsigned tmpu;
	
	int tmpi = this->GetFirstFreeThreadStackBlockId(&tmpu);
	ASSERT(tmpi >= 0 && tmpu == 2); 
	
	tmpi = this->AddThread();
	ASSERT(tmpi >= 0 && tmpi < MaxThreads); 
	
	DEBUG ('a', "Initializing stack register to %d\n",
			numPages * PageSize - 16);

}

//----------------------------------------------------------------------
// AddrSpace::SaveState
//      On a context switch, save any machine state, specific
//      to this address space, that needs saving.
//
//      For now, nothing!
//----------------------------------------------------------------------

	void
AddrSpace::SaveState ()
{
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
//      On a context switch, restore the machine state so that
//      this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

	void
AddrSpace::RestoreState ()
{
	machine->pageTable = pageTable;
	machine->pageTableSize = numPages;
}

//----------------------------------------------------------------------
// AddrSpace::AddThread
//     Ajouter un thread à l'espace d'adressage.
//
//		Dire que la pile du thread ajouté est allouée
//      Incrémenter le nombre de threads créé
//
//		Retourne identifiant unique du thread créé si réussite, 
//		sinon code d'erreur (-1 ou -2, resp. nombre de threads max 
//		par processus atteint, et pile pleine, valeur de 
//		created_thread_id indéfinie si erreur)
//----------------------------------------------------------------------

	int 
AddrSpace::AddThread ()
{

	//Nombre de threads max par processus atteint
	if (threads_created >= MaxThreads)
		return -1;

	unsigned id_in_stack;
	
	addT->AcquireByCurrentThread(); //	début section critique
	
	//	Récupération de l'identifiant dans la pile du premier bloc libre
	//	Test pas de place sur la pile
	if (this->GetFirstFreeThreadStackBlockId(&id_in_stack) < 0){
		addT->ReleaseByCurrentThread(); // fin section critique
		return -2;
	}

	int created_thread_id = threads_created++;
	threads_stack_id[created_thread_id] = id_in_stack;
	addT->ReleaseByCurrentThread();  // fin section critique
	
	stack_blocs[id_in_stack-2] = true;

	return created_thread_id;
}


//----------------------------------------------------------------------
// AddrSpace::RemoveThread
//     Enelever un thread de l'espace d'adressage.
//
//		Dire que la pile du thread ayant l'identifiant unique
//		"unique_thread_id" a été allouée puis désallouée
//		Retourne 0 en cas de succès, et code d'erreur en cas d'echec
//		(-1 si unique_thread_id trop grand, -2 si le thread pas 
//		en cours d'exécution)
//
//----------------------------------------------------------------------

	int
AddrSpace::RemoveThread (int unique_thread_id)
{
	//	Identifiant unique trop grand
	if (unique_thread_id >= MaxThreads)
		return -1;
		
	//~ printf("\nXLe thread #%d (%d) est en pile à #%d\t",unique_thread_id,pid,threads_stack_id[unique_thread_id]-2);
	//	Thread pas en cours d'exécution ou identifiant en pile invalide
	if (threads_stack_id[unique_thread_id] < 2 || threads_stack_id[unique_thread_id] >= MaxRunningThreads)
		return -2;
	threads_stack_id[unique_thread_id] = 1;	
	RunWaitingThread(unique_thread_id);
	stack_blocs[threads_stack_id[unique_thread_id] - 2] = false;
	
	return 0;

}

//----------------------------------------------------------------------
// AddrSpace::RunWaitingThread
//		Cherche le thread ayant l'id "unique_thread_id" dans la liste
//		des threads en attente "de join", l'enlève de cette liste, et le mets
//		dans la liste de threads en attente "d'exécution".
//
//----------------------------------------------------------------------

void AddrSpace::RunWaitingThread(int unique_thread_id){
	unsigned cpt = 0;
	

	waitT->AcquireByCurrentThread();
	while (cpt < waiting_threads.size())
		if (waiting_threads.at(cpt)->forId == unique_thread_id) {
			WaitingThread *tmp = waiting_threads.at(cpt);
			scheduler->ReadyToRun(tmp->who);
			waiting_threads.erase(waiting_threads.begin() + cpt);
			delete tmp;
		}else{
		  cpt++;
		}
	waitT->ReleaseByCurrentThread();
}

//----------------------------------------------------------------------
// AddrSpace::GetStackAddress
//     Retourne l'adresse de la pile d'un thread ayant l'identifiant 
//			unique unique_thread_id
//		Retourne 0 en cas de succès, un code d'erreur sinon: 
//		(-1 si unique_thread_id trop grand, -2 si thread pas en cours 
//		d'execution, -3 si taille max de pile dépassée)
//
//----------------------------------------------------------------------

	int 
AddrSpace::GetStackAddress (unsigned *stack_address, int unique_thread_id)
{
	//Identifiant unique trop grand
	if (unique_thread_id >= MaxThreads)
		return -1;

	//Récupération de l'identifiant dans le pile du thread ayant 
	//	l'identifiant unique unique_thread_id
	unsigned stack_thread_id = threads_stack_id[unique_thread_id];

	//Thread pas en cours d'exécution
	if (stack_thread_id < 2)
		return -2; 

	//Identifiant de thread dépasse la taille maximale de pile
	if (((PageSize*ThreadPages + 16)*(stack_thread_id - 2)) + 16 > UserStackSize)
		return -3;

	*stack_address = numPages * PageSize - ((PageSize*ThreadPages + 16)*(stack_thread_id - 2)) - 16;

	return 0;
}
//----------------------------------------------------------------------
// AddrSpace::GetFirstFreeStackId
//     Retourne l'identifiant du premier emplacement de pile libre
//			disponible pour un thread en commençant de la fin de 
//			l'espace d'adressage
//		Retourne 0 en cas de succès, un code d'erreur sinon
//		(-1 si pas de bloc libre trouvé)
//
//----------------------------------------------------------------------

	int
AddrSpace::GetFirstFreeThreadStackBlockId (unsigned *stack_thread_id)
{
	unsigned offset = 0;
	while (offset<UserStackSize/(PageSize+16/ThreadPages)/ThreadPages) {
		if (!stack_blocs[offset]) {
		 
			*stack_thread_id = offset + 2;
			return 0;
		}
		offset++;
	}

	//Aucun bloc libre trouvé
	return -1;
}
//----------------------------------------------------------------------
// AddrSpace::JoinThread
//		Cette fonctionne ne retourne que si le thread ayant comme
//		identfiant "user_thread_id" se termine.
//		Retourne 0 en cas de succès, code d'erreur sinon (-1 si
//		identifiant jamais créé, -2 si identifiant ne correspond pas
//		à un emplacement actif dans la pile, -3 l'identifiant donné
//		est celui du thread en cours).
//
//----------------------------------------------------------------------

int
AddrSpace::JoinThread (int user_thread_id) {

	if(user_thread_id > threads_created){
		return -1;
	}
	if (threads_stack_id[user_thread_id] == 0){
		return -2;
	}
	if(user_thread_id == currentThread->id){
		return -3;
	}

	if (threads_stack_id[user_thread_id]!=1) { //premier test pour le cas usuel
		IntStatus oldLevel = interrupt->SetLevel (IntOff);
		if (threads_stack_id[user_thread_id]!=1) { //deuxième test pour les cas particuliers

			WaitingThread *waiting_thread = new WaitingThread();
			waiting_thread->who = currentThread;
			waiting_thread->forId = user_thread_id;
			this->waiting_threads.push_back(waiting_thread);

			currentThread->Sleep();
		}
		(void) interrupt->SetLevel (oldLevel);
	}
	return 0;
}

//----------------------------------------------------------------------
// AddrSpace::HasFailed
//		Cette fonction verfie si il y a eu assez de pages physiques
//			pour cet espace d'adressage
//
//----------------------------------------------------------------------

bool
AddrSpace::HasFailed () {
	return assigned_vpn != numPages;
}

//----------------------------------------------------------------------
// AddrSpace::StackIsEmpty
//		Cette fonction vérifie qu'aucun espace dans la pile n'est 
//			reservé
//
//----------------------------------------------------------------------

bool
AddrSpace::StackIsEmpty () {
	unsigned i = 0;
	while(i<MaxRunningThreads && !stack_blocs[i]) 
		i++;
    return i==MaxRunningThreads;
}
