#include "syscall.h"

int main()
{
  char buffer[15];
  PutString("Enter Directory name");
  GetString(buffer,15);
  cd(buffer);
  PutString("Enter Directory name");
  GetString(buffer,15);
  cd(buffer);
  PutString("Enter Directory name");
  GetString(buffer,15);
  cd(buffer);
  return 0;
}
