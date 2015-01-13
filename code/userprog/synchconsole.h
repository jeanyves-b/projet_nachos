#ifdef CHANGED
#ifndef SYNCHCONSOLE_H
#define SYNCHCONSOLE_H
#include "copyright.h"
#include "utility.h"
#include "console.h"
#include "synch.h"
class SynchConsole {
	public:
		SynchConsole(char *readFile, char *writeFile);
		// initialize the hardware console device
		~SynchConsole(); // clean up console emulation
		void SynchPutChar(const char ch); // Unix putchar(3S)
		char SynchGetChar(); // Unix getchar(3S)
		int SynchGetChar2(char* c);
		void SynchPutString(const char *s); // Unix puts(3S)
		void SynchGetString(char *s, int n); // Unix fgets(3S)
		void SynchPutInt(int n); 
		void SynchGetInt(int *n);

	private:
		Semaphore *writing;
		Semaphore *reading;
		Console *console;
		void SPutChar(const char ch);
		char SGetChar();
};
#endif // SYNCHCONSOLE_H
#endif // CHANGED
