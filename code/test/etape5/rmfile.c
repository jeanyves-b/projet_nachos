#include "syscall.h"

int main()
{
  
  char buffer[15];
  GetString(buffer,15);
  if (rmfile(buffer)){
    PutString("file delete");
    return 1;
  }else{
    PutString("Error when deleting file");
    return 0;
  }
  return 1;
  
}
