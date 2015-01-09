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
#include "addrspace.h"
#include "noff.h"

#include <strings.h>		/* for bzero */

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

AddrSpace::AddrSpace (OpenFile * executable)
{
	ASSERT(UserStackSize>=THREAD_PAGES*(PageSize+16));
	
    NoffHeader noffH;
    unsigned int i, size;

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

    DEBUG ('a', "Initializing address space, num pages %d, size %d\n",
	   numPages, size);
// first, set up the translation 
    pageTable = new TranslationEntry[numPages];
    for (i = 0; i < numPages; i++)
      {
	  pageTable[i].virtualPage = i;	// for now, virtual page # = phys page #
	  pageTable[i].physicalPage = i;
	  pageTable[i].valid = TRUE;
	  pageTable[i].use = FALSE;
	  pageTable[i].dirty = FALSE;
	  pageTable[i].readOnly = FALSE;	// if the code segment was entirely on 
	  // a separate page, we could set its 
	  // pages to be read-only
      }

// zero out the entire address space, to zero the unitialized data segment 
// and the stack segment
    bzero (machine->mainMemory, size);

// then, copy in the code and data segments into memory
    if (noffH.code.size > 0)
      {
	  DEBUG ('a', "Initializing code segment, at 0x%x, size %d\n",
		 noffH.code.virtualAddr, noffH.code.size);
	  executable->ReadAt (&(machine->mainMemory[noffH.code.virtualAddr]),
			      noffH.code.size, noffH.code.inFileAddr);
      }
    if (noffH.initData.size > 0)
      {
	  DEBUG ('a', "Initializing data segment, at 0x%x, size %d\n",
		 noffH.initData.virtualAddr, noffH.initData.size);
	  executable->ReadAt (&
			      (machine->mainMemory
			       [noffH.initData.virtualAddr]),
			      noffH.initData.size, noffH.initData.inFileAddr);
      }

	//	initialisation des variables de gestion des threads de l'espace
	//		d'adressage
	threads_stack = new bool[numPages/THREAD_PAGES];
	unsigned int threads_max = UserStackSize/(PageSize+16)/THREAD_PAGES;
	for(unsigned int j=0; j<threads_max; j++)
		threads_stack[j] = false;
	threads_join = new Semaphore*[numPages/THREAD_PAGES];
	mut = new Lock("Addr Lock");
	nbt = 0;
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
//      Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace ()
{
  int i;
  // LB: Missing [] for delete
  // delete pageTable;
  delete [] pageTable;
  // End of modification
  delete threads_stack;
  for (i=0;i<(numPages/THREAD_PAGES);i++){
    if (thread_join[i] != NULL){
      delete thread_join[i];
    }
  }
  delete thread_join;
  delete mut;
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
    
    ASSERT(this->AddThread() == 0); //
    DEBUG ('a', "Initializing stack register to %d\n",
	   numPages * PageSize - 16);
    
    thread_join[0] = new Semaphore("First Thread",1);
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
//      Incrémenter le nombre de threads
//		Retourner l'identifiant du thread ajouté
//----------------------------------------------------------------------

int 
AddrSpace::AddThread ()
{
	int id = -1;
	mut->Acquire();
	 = this->GetFirstFreeThreadStackBlockId();
	if (id<0)
		return -1;
	
	if (nbt == 0){
	  thread_join[0]->V;//sécurité
	}
	threads_stack[id] = true;
	if (thread_join[id] != NULL){
	  delete thread_join[id];
	}
	thread_join[id] = new Semaphore("new Thread",0);
	nbt++;
	mut->Release();
    return id;
}


//----------------------------------------------------------------------
// AddrSpace::RemoveThread
//     Enelever un thread de l'espace d'adressage.
//
//		Dire que la pile du thread "thread_id" est non allouée
//      Décrémenter le nombre de threads
//
//----------------------------------------------------------------------

void
AddrSpace::RemoveThread (int thread_id)
{
	threads_stack[thread_id] = false;
	mut->Acquire();
	nbt--; //Section critique
	if (nbt == 0){
	  thread_join[0]->V;
	}
	mut->Release();
	thread_join[id]->V;//on signale notre arrêt
}

//----------------------------------------------------------------------
// AddrSpace::GetStackAddress
//     Retourne l'adresse de la pile d'un thread ayant l'identifiant 
//			threadId
//
//----------------------------------------------------------------------

int
AddrSpace::GetStackAddress (int threadId)
{
    return UserStackSize - ((PageSize+16)*THREAD_PAGES*threadId);
}
//----------------------------------------------------------------------
// AddrSpace::GetFirstFreeStackId
//     Retourne l'identifiant du premier emplacement de pile libre
//			disponible pour un thread en commençant de la fin de 
//			l'espace d'adressage
//
//----------------------------------------------------------------------

int
AddrSpace::GetFirstFreeThreadStackBlockId ()
{
	unsigned offset = 0;
    while (offset<UserStackSize/(PageSize+16)/THREAD_PAGES) {
		if (!threads_stack[offset])
			return offset;
		offset++;
	}
	
	return -1;
}

void AddrSpace::Join(unsigned id){
  ASSERT(id < (int)(numPages/THREAD_PAGES));
  ASSERT(id >= 0);
  ASSERT(threads_stack[id] = TRUE);
  if (id == 0){
    ASSERT(currentThread->id == 0);
  }else{
    ASSERT(id != currentThread->id);
  }
  thread_join[id]->P;//on attend la fin du thread
  thread_join[id]->V;//on a attesté de la fin du thread et on doit continuer de la signaler
}
