#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void stringParsing(char **cmd, char *line, int *wordsInCmd);
void promptLine(char *line);


int main(){

  char line[100], **cmd;
  int wordsInCmd;
  promptLine(line);
  stringParsing(cmd, line, &wordsInCmd);


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


void stringParsing(char **cmd, char *line, int *wordsInCmd){

  int nwords = 0, count = 0;
  char *temp;
  // nwords por defecto en 2, la primera palabra y el NULL del final
  
  bool inSpace = true;
  while(line[count] != '\0'){      // Contamos palabras
    if(line[count] == ' '){
      inSpace = true;
    } 
    else if (inSpace){
      inSpace = false;
      nwords++;
    }
    count++;
  }
  //EndWhile
  
  *wordsInCmd = nwords;

  printf("Palabras: %d\n", nwords);

  // Pedimos espacio para punteros de las palabras con malloc
  cmd = (char **)malloc(nwords*sizeof(char *));

  count = 0;
  while((temp = strsep(&line, " ")) != NULL){
    printf("Temp: %s\n", temp);
    cmd[count] = strdup(temp);
    count++;
  }
  
  cmd[nwords - 1] = NULL;    // Dejamos el extra como NULL


  
  //Imprimir palabras separadas
  printf("Palabras pareseadas\n");
  for(int i = 0; i < nwords; i++){
    if(cmd[i] != NULL)
      printf("%s\n", cmd[i]);
    else if(cmd[i] == NULL){
      printf("Nulo en pos %d\n", i);
    }
  }
  
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
