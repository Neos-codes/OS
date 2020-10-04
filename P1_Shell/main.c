#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

using namespace std;


int stringParsing(char **cmd, char *line);

bool promptLine(char *line);


int main(){

  char line[100];
  char *cmd[100];
  int wordsInCmd;

  bool running = promptLine(line);
  while (running){
    wordsInCmd = stringParsing(cmd, line);
    
    //printf("N palabras desde main: %d\n", wordsInCmd);
    //cout<<"N palabras desde main: "<<wordsInCmd<<endl;

    //printf("Desde main:\n");
    /*cout<<"Desde main: \n";
    for(int i = 0; i < wordsInCmd; i++){
      printf("%s\n", cmd[i]);
    }*/

    pid_t pid = fork();
    
    if(pid < 0){
      //printf("Error al crear hijo!\n");
      cout<<"Error al crear hijo!\n";
      exit(0);
    }
    else if(pid == 0){       // Hijo
      //printf("Hijo creado\n");
      cout<<"Hijo creado\n";
      //char *cmds[3] = {"ls", "-n", NULL};
      execvp(cmd[0], cmd);
      //printf("Error al ejecutar comando!\n");
      cout<<"Error al ejecutar comando!\n";
      //exit(0);
    }
    else{                   // Padre
      wait(NULL);
      //exit(0);
    }
    running = promptLine(line);
  }
  
  return 0;
}


int stringParsing(char **cmd, char *line){

  int nwords = 0, count = 0;
  char *temp;
  // nwords por defecto en 2, la primera palabra y el NULL del final


  bool inSpace = true;
  while(line[count] != '\0'){      // Contamos palabras
    if(line[count] == ' '){
      inSpace = true;
    } 
    else if(inSpace){
      nwords++;
      inSpace = false;
    }
    count++;
  }//EndWhile

  //printf("Palabras: %d\n", nwords);

  // Pedimos espacio para punteros de las palabras con malloc
  //cmd = (char **)malloc((nwords+1)*sizeof(char *));

  count = 0;
  //cout<<"problem?\n";
  while(1){


    cmd[count] = (temp = strsep(&line, " "));
    //cout<<cmd[count]<<" WITH "<<count<<endl;

    //printf("%s WITH %d\n", cmd[count], count);
    //printf("%s\n", cmd[count]);

    if (cmd[count] == NULL){
      break;
    }
    if (strlen(cmd[count]) == 0){
      continue;
    }
    count++;
  }
  /*cout<<"Fin\n";
  for(int i = 0; i < nwords; i++){
    cout<<cmd[i]<<endl;
  }*/
  
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

bool promptLine(char *line){
  
  do{                                            // Vuelve a preguntar mientras
    //printf(">>> ");                         // Se aprete Enter
    cout<<"> ";
    fgets(line, 100, stdin);
  }while(line[0] == '\n');


  //NEEDLESS SPACES START

  line[strlen(line) - 1] = '\0';



  //NEEDLESS SPACES END
  if (!strcmp(line, "exit")){
    cout<<"Exiting, cya l8r aligator\n";
    return false;
  }

  /*
  for(int i = 0; i < strlen(line); i++){
    if(line[i] == '\n')
      //printf("En pos i: %d", i);
      cout<<"En pos i: "<<i<<endl;
  }*/
  return true;
  //printf("Prompt leyo: %s\n", line);
}
