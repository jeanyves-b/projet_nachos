#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"
static Semaphore *readAvail;
static Semaphore *writeDone;
static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
  readAvail = new Semaphore("read avail", 0);
  writeDone = new Semaphore("write done", 0);
  console = new Console(readFile,writeFile,ReadAvail,WriteDone,0);
}

SynchConsole::~SynchConsole()
{
  delete console;
  delete writeDone;
  delete readAvail;
}

void SynchConsole::SynchPutChar(const char ch)
{
    console->PutChar (ch);	// echo it!
    writeDone->P ();	// wait for write to finish
}

char SynchConsole::SynchGetChar()
{
    readAvail->P ();	// wait for character to arrive
    return console->GetChar ();
}

void SynchConsole::SynchPutString(const char s[])
{
	int i = 0;
  while (s[i] != '\0'){
    SynchPutChar(s[i]);
    i++;
  }
  SynchPutChar('\n');
}

void SynchConsole::SynchGetString(char *s, int n)
{
	char current_char;
	int i = 0;
	do {
	
      current_char = SynchGetChar();
      s[i] = current_char;
      i++;
    } while (i < n && current_char != '\n');
    
    //vider le buffer noyau pour eviter les fuits noyau
    if (i == n) {
		do {
			current_char = SynchGetChar();
		} while (current_char != '\n'); 
	} else {
		s[i] = '\0';
	}
}

#endif // CHANGED
