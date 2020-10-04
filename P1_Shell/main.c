#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


int stringParsing(char **cmd, char *line);

void promptLine(char *line);


int main(){

  char line[100], **cmd;
  int wordsInCmd;
  promptLine(line);
  wordsInCmd = stringParsing(cmd, line);
  
  printf("N palabras desde main: %d\n", wordsInCmd);

  printf("Desde main:\n");
  for(int i = 0; i < wordsInCmd; i++){
    printf("%s\n", cmd[i]);
  }

  pid_t pid = fork();
  
  if(pid < 0){
    printf("Error al crear hijo!\n");
    exit(0);
  }
  else if(pid == 0){       // Hijo
    printf("Hijo creado\n");
    //char *cmds[3] = {"ls", "-n", NULL};
    execvp(cmd[0], cmd);
    printf("Error al ejecutar comando!\n");
    //exit(0);
  }
  else{                   // Padre
    wait(NULL);
    exit(0);
  }
  
  return 0;
}


int stringParsing(char **cmd, char *line){

  int nwords = 0, count = 0;
  char *temp;
  // nwords por defecto en 2, la primera palabra y el NULL del final


  int inSpace = 1;
  while(line[count] != '\0'){      // Contamos palabras
    if(line[count] == ' '){
      inSpace = 1;
    } 
    else if(inSpace){
      nwords++;
      inSpace = 0;
    }
    count++;
  }//EndWhile

  //printf("Palabras: %d\n", nwords);

  // Pedimos espacio para punteros de las palabras con malloc
  cmd = (char **)malloc((nwords+1)*sizeof(char *));

  count = 0;
  printf("problem?\n");
  while((temp = strsep(&line, " ")) != NULL){


    cmd[count] = (char *)malloc(strlen(temp)*sizeof(char));
    strcpy(cmd[count], temp);
    printf("%s\n", cmd[count]);

    count++;
  }
  printf("Fin\n");
  for(int i = 0; i < nwords; i++){
    printf("%s\n", cmd[i]);
  }
  
  cmd[nwords] = NULL;    // Dejamos el extra como NULL


  
  //Imprimir palabras separadas
  /*printf("Palabras pareseadas\n");
  for(int i = 0; i < nwords; i++){
    if(cmd[i] != NULL)
      printf("%s\n", cmd[i]);
    else if(cmd[i] == NULL){
      printf("Nulo en pos %d\n", i);
    }
    }*/

  return nwords;
}

void promptLine(char *line){
  
  do{                                            // Vuelve a preguntar mientras
    printf(">>> ");                         // Se aprete Enter
    fgets(line, 100, stdin);
  }while(line[0] == '\n');
  line[strlen(line) - 1] = '\0';

  for(int i = 0; i < strlen(line); i++){
    if(line[i] == '\n')
      printf("En pos i: %d", i);
  }
  //printf("Prompt leyo: %s\n", line);
}
