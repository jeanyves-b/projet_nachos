#include "syscall.h"

int main()
{
  
  char buffer[15];
  GetString(buffer,15);
  if (mkfile(buffer,128)){
    PutString("file is created");
    return 1;
  }else{
    PutString("Error when creating file");
    return 0;
  }
  return 1;
  
}
