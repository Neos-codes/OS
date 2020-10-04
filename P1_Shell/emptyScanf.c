#include <stdio.h>

int main(){

  char str[20];
  int num;
  char temp = '\n';
  
  fgets(str, sizeof(str), stdin);
  printf("Lei el signo es ascii %d\n", (int)str[0]);   // (int)'\n' = 10
  printf("el /n = %d\n", (int)temp);
  
  return 0;
}
