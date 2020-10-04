#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

  pid_t pid;
  pid = fork();

  if(pid < 0){      // Error al crear hijo
    printf("Error al crear hijo!\n");
    exit(0);
  }
  else if(!pid){    // Hijo
    char *cmd[3] = {"ls", "-n", NULL};
    execvp(cmd[0], cmd);
    printf("Error ejecutar comando!\n");
  }
  else{                // Padre
    wait(NULL);
    exit(0);
  }
  return 0;
}
