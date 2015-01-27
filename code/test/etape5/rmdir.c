#include "syscall.h"

int main()
{
  char buffer[15];
  PutString("Enter the directory to delete");
  GetString(buffer,15);
  if (rmdir(buffer)){
    PutString("Directory is deleted");
    return 1;
  }else{
    PutString("Error when deleting directory");
    return 0;
  }
}
