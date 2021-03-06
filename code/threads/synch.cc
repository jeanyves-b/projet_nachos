// synch.cc 
//      Routines for synchronizing threads.  Three kinds of
//      synchronization routines are defined here: semaphores, locks 
//      and condition variables (the implementation of the last two
//      are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
//      Initialize a semaphore, so that it can be used for synchronization.
//
//      "debugName" is an arbitrary name, useful for debugging.
//      "initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore (const char *debugName, int initialValue)
{
	DEBUG('s', "Creating Semaphore \"%s\" with value %d\n", debugName, initialValue);
	name = debugName;
	value = initialValue;
	queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
//      De-allocate semaphore, when no longer needed.  Assume no one
//      is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore ()
{
	DEBUG('s', "Destroying Semaphore \"%s\"\n", name);
	delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
//      Wait until semaphore value > 0, then decrement.  Checking the
//      value and decrementing must be done atomically, so we
//      need to disable interrupts before checking the value.
//
//      Note that Thread::Sleep assumes that interrupts are disabled
//      when it is called.
//----------------------------------------------------------------------

	void
Semaphore::P ()
{
	DEBUG('s', "Wait request on Semaphore \"%s\" who has value %d\n", name, value);
	IntStatus oldLevel = interrupt->SetLevel (IntOff);	// disable interrupts

	while (value == 0)
	{				// semaphore not available
		queue->Append ((void *) currentThread);	// so go to sleep
		currentThread->Sleep ();
	}
	value--;			// semaphore available, 
	// consume its value

	(void) interrupt->SetLevel (oldLevel);	// re-enable interrupts
	DEBUG('s', "Successful wait request on Semaphore \"%s\" who has now value %d\n", name, value);
}

//----------------------------------------------------------------------
// Semaphore::V
//      Increment semaphore value, waking up a waiter if necessary.
//      As with P(), this operation must be atomic, so we need to disable
//      interrupts.  Scheduler::ReadyToRun() assumes that threads
//      are disabled when it is called.
//----------------------------------------------------------------------

	void
Semaphore::V ()
{
	DEBUG('s', "Post request on Semaphore \"%s\" who has value %d\n", name, value);
	Thread *thread;
	IntStatus oldLevel = interrupt->SetLevel (IntOff);

	thread = (Thread *) queue->Remove ();
	if (thread != NULL)		// make thread ready, consuming the V immediately
		scheduler->ReadyToRun (thread);
	value++;
	(void) interrupt->SetLevel (oldLevel);
	DEBUG('s', "Post request successful on Semaphore \"%s\" who has now value %d\n", name, value);
}

// Dummy functions -- so we can compile our later assignments 
// Note -- without a correct implementation of Condition::Wait(), 
// the test case in the network assignment won't work!
Lock::Lock (const char *debugName)
{
	DEBUG('l', "Creating lock %s\n", debugName);
	name = debugName;
	lock = new Semaphore("mutex",1);
}

Lock::~Lock ()
{
	DEBUG('l', "Destroying lock %s\n", name);
	delete lock;
}
	void
Lock::Acquire ()
{		
	DEBUG('l', "Trying to acquire lock \"%s\" by thread %d pid %d \n", name, tid, pid);
	lock->P();
	tid = currentThread->id;
	pid = currentThread->getPid();
	DEBUG('l', "Lock \"%s\" successfully acquired by thread %d pid %d \n", name, tid, pid);
}
	void
Lock::Release ()
{
	DEBUG('l', "Trying to release lock \"%s\" by thread %d pid %d \n", name, tid, pid);
	if (isHeldByCurrentThread()){
		tid = -1;
		pid = -1;
		lock->V();
		DEBUG('l', "Lock \"%s\" successfully released by thread %d pid %d \n", name, tid, pid);
	}
	DEBUG('l', "Lock \"%s\" is not held by thread %d pid %d to be released\n", name, tid, pid);
}

bool Lock::isHeldByCurrentThread(){
	return (tid == currentThread->id && pid == (currentThread->getPid()));
}

Condition::Condition (const char *debugName)
{
	name = debugName;
	waiting = new List;
}

Condition::~Condition ()
{
	delete waiting;
}
	void
Condition::Wait (Lock * conditionLock)
{
	if(conditionLock->isHeldByCurrentThread()) {
		IntStatus oldLevel = interrupt->SetLevel (IntOff); // disable interrupts
		
		waiting->Append((void*)currentThread); //	ajouter à la liste des threads qui attendent sur la condition
		conditionLock->Release(); // libérer le lock
		currentThread->Sleep();  //	attendre un signal/broadcast
		
		(void) interrupt->SetLevel (oldLevel);	// re-enable interrupts
		
		DEBUG('l', "Thread %d pid %d is waiting with condition \"%s\" on lock \"%s\"\n",
			conditionLock->getTid(),  conditionLock->getPid(), name, conditionLock->getName());
	}
}

	void
Condition::Signal (Lock * conditionLock)
{
	IntStatus oldLevel = interrupt->SetLevel(IntOff); // disable interrupts

	Thread* thread = (Thread*)waiting->Remove(); //	Récupérer un thread qui est en attente
	if(thread != NULL)
		scheduler->ReadyToRun(thread); //	ajouter le thread reveillé à la liste des threads à exécuter
		
    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

	void
Condition::Broadcast (Lock * conditionLock)
{
	while (!waiting->IsEmpty()) {
        this->Signal(conditionLock);
    }
}
