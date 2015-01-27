// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "limits.h"
#include "userthread.h"
#include "userprocess.h"

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
	static void
UpdatePC ()
{
	int pc = machine->ReadRegister (PCReg);
	machine->WriteRegister (PrevPCReg, pc);
	pc = machine->ReadRegister (NextPCReg);
	machine->WriteRegister (PCReg, pc);
	pc += 4;
	machine->WriteRegister (NextPCReg, pc);
}


//----------------------------------------------------------------------
// copyStringFromMachine: fonction pour copier une chaine du monde MIPS
// 	vers le monde Linux
//----------------------------------------------------------------------
void copyStringFromMachine(int from, char *to, unsigned size)
{
	bool test = 0;
	int tmp;
	for (unsigned i=0 ; i<size ; i++)
	{
		machine->ReadMem(from+i, 1, &tmp);
		to[i] = (char) tmp;
		if (to[i] == EOF || to[i] == '\n' || to[i] == '\r' || to[i] == '\0')
		{
			test = true;
			to[i] = '\0';
			break;
		}
	}
	if(test)
	{
		to[size-1] = '\0';
	}
}

//----------------------------------------------------------------------
// copyStringToMachine: fonction pour copier une chaine du monde Linux
// 	vers le monde MIPS.
//----------------------------------------------------------------------
void copyStringToMachine(char* from, int to, unsigned size)
{
	unsigned i =0;
	for(; i<size && from[i]!='\0' 
			&& from[i] != '\n' 
			&& from[i] != '\r'
			&& from[i] != EOF; i++) {
		machine->WriteMem(to+i, 1, (int)(from[i]));
	}
	machine->WriteMem(to+i, 1, (int)('\0'));
}

//----------------------------------------------------------------------
// writeString: fonction pour écrire une chaine du 
// monde MIPS sur la console
//----------------------------------------------------------------------

void writeString(int from){
	char to[MaxStringSize];
	copyStringFromMachine(from,to,MaxStringSize);
	synchconsole->SynchPutString(to);
}

//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

	void
ExceptionHandler (ExceptionType which)
{
	int type = machine->ReadRegister (2);

	if ((which == SyscallException))
	{
		switch (type) {
			case SC_Halt: {
					      DEBUG('a', "Shutdown, initiated by user program.\n");
					      interrupt->Halt();
					      break;
				      }
			case SC_Exit: {
						DEBUG ('r', "Thread \"%s %d (pid: %d)\" called Exit\n",currentThread->getName(), currentThread->id, currentThread->space->pid);
						do_UserProcessExit();
					    break;
				      }
			case SC_PutChar: {
						 DEBUG('c', "Writing character on standard output, initiated by user program.\n");
						 synchconsole->SynchPutChar(machine->ReadRegister(4));
						 break;
					 }
			case SC_PutString:{ 
						DEBUG('c', "Writing a string on standard output, initiated by user program.\n");
						writeString(machine->ReadRegister(4));
						break;
					}	
			case SC_GetChar: { 
						 DEBUG('c', "reading character on standard intput, initiated by user program.\n");
						 machine->WriteRegister(2,(int)synchconsole->SynchGetChar());
						 break;
					 }
			case SC_GetString: { 
						DEBUG('c', "reading string from standard intput, initiated by user program.\n");
						 
						if (machine->ReadRegister(5) > 0) {
							 
							unsigned chars_nb;
							
							//récupération du nombre de caractères à récupérer dans les limites de la taille maximum d'une chaine
							if ((unsigned) machine->ReadRegister(5) < MaxStringSize) {
								chars_nb = (unsigned) machine->ReadRegister(5);
							} else {
								chars_nb = MaxStringSize;
							}
							
							char* buf = new char[chars_nb];
							synchconsole->SynchGetString(buf, chars_nb); //	récupération de la chaine depuis la console

							 //récupération du nombre de caractères à copier dans les limites des caractères d'arrêt
							 unsigned chars_to_copy = 0;	
							 for(; chars_to_copy < chars_nb 
									 && buf[chars_to_copy] != '\n'
									 && buf[chars_to_copy] != '\r'
									 && buf[chars_to_copy] != '\0'
									 && buf[chars_to_copy] != EOF;
									 chars_to_copy++);
									 
							 unsigned copied_chars = 0; //	caractères déjà copiés
							 //	copie des caractères par blocs de MaxStringSize vers la machine
							 for (;	chars_to_copy >= MaxStringSize;
									 chars_to_copy -= MaxStringSize,
									 copied_chars += MaxStringSize) {
								 copyStringToMachine(buf + copied_chars, machine->ReadRegister(4) + copied_chars, MaxStringSize);
							 }
							 //	copie du reste des caractères vers la machine
							 if (chars_to_copy>0) {
								 copyStringToMachine(buf + copied_chars, machine->ReadRegister(4) + copied_chars, chars_to_copy);
							 }
							 delete [] buf;
						 }
						 
						 break; 
					 }
			case SC_PutInt: { 
						 DEBUG('c', "writing signed integer on standard output, initiated by user program.\n");
						 synchconsole->SynchPutInt(machine->ReadRegister(4));
						 break;
					 }
			case SC_GetInt: { 
						 DEBUG('c', "reading signed integer from standard intput, initiated by user program.\n");
						 machine->WriteRegister(2, synchconsole->SynchGetInt());
						 break;
					 }
			case SC_UserThreadC: { //UserThreadCreate
						 DEBUG('t', "Creating user thread, initiated by user program.\n");
						 machine->WriteRegister(2, do_UserThreadCreate(machine->ReadRegister(4), machine->ReadRegister(5), machine->ReadRegister(8)));
						 break;
					 }
			case SC_UserThreadE: { //UserThreadExit
						 DEBUG('t', "Exiting current thread, initiated by user program.\n");
						 do_UserThreadExit();
						 break;
					 }
			case SC_UserThreadJ: { //UserThreadJoin
						 DEBUG('t', "Joining a thread, initiated by user program.\n");
						 machine->WriteRegister(2, currentThread->Join(machine->ReadRegister(4)));
						 break;
					 }
			case SC_GetTid: {
						machine->WriteRegister(2, currentThread->id);
						break;
				}
			case SC_ForkExec:{ //ForkExec
						 DEBUG('a', "Starting a new process, initiated by user program.\n");
						 char buf[MaxStringSize];
						 copyStringFromMachine(machine->ReadRegister(4),buf,MaxStringSize);
						 machine->WriteRegister(2, do_UserProcessCreate(buf));
						 break;
					 }
			case SC_GetPid: {
						machine->WriteRegister(2, currentThread->getPid());
						break;
				}
			case SC_Send: {
					unsigned size = (unsigned)machine->ReadRegister(5);
					if (size > MaxStringSize) {
						machine->WriteRegister(2, 0);
						break;
					}
					
					char *buffer = new char[size];
					copyStringFromMachine(machine->ReadRegister(4),buffer,size);
					unsigned sent = postOffice->SendUnfixedSize(buffer, size, machine->ReadRegister(6), machine->ReadRegister(7), machine->ReadRegister(8));
					delete [] buffer;
					
					machine->WriteRegister(2, sent);
					break;
				}
			case SC_Receive: {
					unsigned size = (unsigned)machine->ReadRegister(6);
					if (size > MaxStringSize) {
						break;
					}
					char *buffer = new char[size];
					postOffice->ReceiveUnfixedSize(machine->ReadRegister(4), buffer, size);
					copyStringToMachine(buffer, machine->ReadRegister(5), size);
					delete [] buffer;
					break;
				}
			case SC_SendFile: {
					char *buffer = new char[MaxStringSize];
					copyStringFromMachine(machine->ReadRegister(4),buffer,MaxStringSize);
					machine->WriteRegister(2, postOffice->SendFile(buffer, machine->ReadRegister(5), machine->ReadRegister(6), machine->ReadRegister(7)));
					delete [] buffer;
					break;
				}
			case SC_ReceiveFile: {
					char *buffer = new char[MaxStringSize];
					copyStringFromMachine(machine->ReadRegister(5),buffer,MaxStringSize);
					machine->WriteRegister(2, postOffice->ReceiveFile(machine->ReadRegister(4), buffer));
					delete [] buffer;
					break;
				}
			case SC_Sleep: {
					Delay (machine->ReadRegister(4));
					break;
				}
			default: {
					 printf("Unexpected user mode exception %d %d\n", which, type);
					 ASSERT(FALSE);
				 }
		}
	}
	else if (which == AddressErrorException)
	{
		printf ("Address translation exception %d\n", type);
		ASSERT (FALSE);
	}
	else
	{
		printf ("Unexpected user mode exception %d %d\n", which, type);
		ASSERT (FALSE);
	}

	// LB: Do not forget to increment the pc before returning!
	UpdatePC ();
	// End of addition
}
