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

int SynchConsole::SynchGetChar2(char* c)
{
    readAvail->P ();	// wait for character to arrive
    *c = console->GetChar ();
    if (*c == EOF){
      return -1;
    }else{
      return 0;
    }
   
}

void SynchConsole::SynchPutString(const char s[])
{
  while ((char)*s != '\0'){
    SynchPutChar(*s);
    s++;
  }
  SynchPutChar('\n');
}

void SynchConsole::SynchGetString(char *s, int n)
{
    while (n > 0){
      *s = SynchGetChar();
      s++;
      n--;
    }
}

#endif // CHANGED