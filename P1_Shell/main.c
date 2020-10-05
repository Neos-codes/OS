#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

using namespace std;


/*BUGS VERSION 0.01 RODOLFO



*/

/*FIXED
EXIT NOT WORKING PROPERLY

*/

/*ADDEd
working CD


*/


int stringParsing(char **cmd, char *line);

bool promptLine(char *line);

void executeProgram(char *cmd[100], int words, FILE *fp);


int main(){

  char line[100];
  char *cmd[100];
  int wordsInCmd;
  
  FILE *fp = fopen("mishell.log", "a+");      // Abrir mishell.log
  fseek(fp, 0, SEEK_END);
  if(fp == NULL)
    cout << "Problema al abrir mishell.log" << endl;

  system("clear");

  bool running = promptLine(line);
  while (running){
    
    wordsInCmd = stringParsing(cmd, line);

    for(int i = 0; i < wordsInCmd; i++){     // Escribir linea en mishell.log
      fprintf(fp, "%s ", cmd[i]);
    }
    fprintf(fp, "\n");
    
    //printf("N palabras desde main: %d\n", wordsInCmd);
    //cout<<"N palabras desde main: "<<wordsInCmd<<endl;

    //printf("Desde main:\n");
    /*cout<<"Desde main: \n";
    for(int i = 0; i < wordsInCmd; i++){
      printf("%s\n", cmd[i]);
    }*/

    //Check if function is built in
    if (!strcmp(cmd[0], "exit")){
      cout<<"Exiting, cya l8r aligator\n";
      return 0;
    }

    if (!strcmp(cmd[0], "cd")){
      chdir(cmd[1]);
      goto end;
    }


    //Run program
    executeProgram(cmd, wordsInCmd, fp);
    end:
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
  /* printf("Palabras pareseadas\n");
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
  char curDir[1024];
  char curUsr[64];
  char curHst[64];
  do{                                            
  // Vuelve a preguntar mientras se aprete Enter
    getcwd(curDir, sizeof(curDir));      // Imprimir prompt
    getlogin_r(curUsr, sizeof(curUsr));
    gethostname(curHst, sizeof(curHst));
    
    cout<<"\033[1;31m"<<curUsr<<"@"<<curHst<<"\033[0m:\033[1;36m~"<<get_current_dir_name()<<" "<<getpid()<<"\033[0m > ";
    fgets(line, 100, stdin);                   // Leer linea
  }while(line[0] == '\n');
  line[strlen(line) - 1] = '\0';
  
  /*
  for(int i = 0; i < strlen(line); i++){
    if(line[i] == '\n')
      cout<<"En pos i: "<<i<<endl;
  }*/
  return true;
  //printf("Prompt leyo: %s\n", line);
}

void executeProgram(char *cmd[100], int words, FILE *fp){

  pid_t pid = fork();
    
  if(pid < 0){
    cout<<"Error al crear hijo!\n";
    exit(0);
  }
  else if(pid == 0){       // Hijo
    //cout<<"Hijo creado\n";
    //char *cmds[3] = {"ls", "-n", NULL};
    execvp(cmd[0], cmd);
    cout<<"Error al ejecutar ";
    for(int i = 0; i < words; i++)
      cout << cmd[i] << " ";
    cout << endl;
    exit(0);
  }
  else{                   // Padre
    wait(NULL);
    //exit(0);
  }
}
