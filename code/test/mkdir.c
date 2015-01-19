#include "syscall.h"

int main()
{
  char buffer[15];
  GetString(buffer,15);
  if (mkdir(buffer)){
    PutString("Directory is created");
    return 1;
  }else{
    PutString("Error when creating directory");
    return 0;
  }
}