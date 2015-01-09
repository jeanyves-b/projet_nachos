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
  writing = new Semaphore("writing", 0);
  reading = new Semaphore("reading", 0);
  console = new Console(readFile,writeFile,ReadAvail,WriteDone,0);
}

SynchConsole::~SynchConsole()
{
  delete console;
  delete writeDone;
  delete readAvail;
  delete reading;
  delete writing;
}

void SynchConsole::SPutChar(const char ch)
{
    console->PutChar (ch);	// echo it!
    writeDone->P ();	// wait for write to finish
}

void SynchConsole::SynchPutChar(const char ch)
{
	writing->P();
	SPutChar(ch);
    writing->V();
}

char SynchConsole::SGetChar()
{
    readAvail->P ();	// wait for character to arrive
    return console->GetChar ();
}

char SynchConsole::SynchGetChar()
{
	reading->P();
	char ch = SGetChar();
	reading->V();
	return ch;
}

int SynchConsole::SynchGetChar2(char* c)
{
	reading->P();
    readAvail->P ();	// wait for character to arrive
    *c = console->GetChar ();
    reading->V();
    if (*c == EOF){
      return -1;
    }else{
      return 0;
    }
   
}

void SynchConsole::SynchPutString(const char s[])
{
	writing->P();
	int i = 0;
  	while (s[i] != '\0'){
    	this->SPutChar(s[i]);
    	i++;
	}
	this->SPutChar('\n');
	writing->V();
}

void SynchConsole::SynchGetString(char *s, int n)
{
	reading->P();
	char current_char;
	int i = 0;
	do {
	
      current_char = this->SGetChar();
      s[i] = current_char;
      i++;
    } while (i < n && current_char != '\n');
    
    //vider le buffer noyau pour eviter les fuits noyau
    if (i == n) {
		do {
			current_char = this->SGetChar();
		} while (current_char != '\n'); 
	} else {
		s[i] = '\0';
	}
	reading->V();
}

void SynchConsole::SynchPutInt(int n)
{
	//	récupération du nombre de chiffres de l'entier
	//	+1 si négative, et +1 pour le byte null
	int digits = 1, tmp = n;
	if (tmp == 0) 
		digits = 2;
	else {
		if (tmp < 0) {
			tmp = -tmp;
			digits++;
		}
		for (; tmp > 0; digits++, tmp = tmp / 10);
	}	
	
	//écriture de l'entier à l'emplacement buf en mémoire
	char* buf = new char[digits];
	snprintf(buf, digits, "%d", n);
	buf[digits-1] = '\0';
	
	this->SynchPutString(buf);
}

void SynchConsole::SynchGetInt(int *n)
{
	//récupération d'un nombre dans buf
	char* buf = new char[MAX_STRING_SIZE];
	this->SynchGetString(buf, MAX_STRING_SIZE);
	sscanf(buf, "%d", n);
}

#endif // CHANGED
