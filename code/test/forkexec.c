#include "syscall.h"
int main()
{
SynchPutString("entring in main");
ForkExec("putint");
//~ ForkExec("userpages1");

return 0;
}
