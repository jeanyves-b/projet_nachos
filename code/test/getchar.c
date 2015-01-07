#include "syscall.h"

int main()
{
char c;
while (c!='q') {
	c = SynchGetChar();
	PutChar(c);
}
	
return 0;

}
