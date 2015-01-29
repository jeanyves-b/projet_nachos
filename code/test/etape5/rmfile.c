#include "syscall.h"

int main()
{
  
  char buffer[15];
  PutString("Enter the file to delete:");
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
