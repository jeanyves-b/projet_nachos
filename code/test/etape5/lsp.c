#include "syscall.h"

int main()
{
  char buffer[15];
  PutString("Enter directory path");
  GetString(buffer,15);
  ls(buffer);

  return 0;
}
