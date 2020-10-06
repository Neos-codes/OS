#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>
#include <string>


using namespace std;

struct entry {
        char cmd[100];
        int rep;
};

int stringParsing(char **cmd, char *line);

bool promptLine(char *line);

void executeProgram(char *cmd[100]){
  pid_t pid = fork();

  if(pid < 0){
    //printf("Error al crear hijo!\n");
    cout<<"Error al crear hijo!\n";
    exit(0);
  }
  else if(pid == 0){       // Hijo
    //printf("Hijo creado\n");
    //cout<<"Hijo creado\n";
    //char *cmds[3] = {"ls", "-n", NULL};
    execvp(cmd[0], cmd);
    //printf("Error al ejecutar comando!\n");
    cout<<"Error al ejecutar comando!\n";
    exit(0);
  }
  else{                   // Padre
    wait(NULL);
    //exit(0);
  }
}

int update_freq (char *line, vector <pair<int, char *> > &command_freq) {
    for (int i = 0; i < command_freq.size(); i++) {
        if (strcmp(line, command_freq[i].second) == 0) {
            command_freq[i].first++;
            return 0;
        }
    }
    pair<int, char *> aux;
    aux.first = 1;
    aux.second = strdup(line);
    command_freq.push_back(aux);
    // printf("%s check!\n", command_freq.back().second);
    return 0;
}


int main(){

    vector <pair<int, char *> > command_freq;
    char line[100];
    char *cmd[100];
    int wordsInCmd;
    system("clear");
    bool running = promptLine(line);

    while (running){
        update_freq (line, command_freq);
        wordsInCmd = stringParsing(cmd, line);

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
        if (!strcmp(cmd[0], "revisarVector")){ //for debug
            cout << "en el vector hay " << command_freq.size() <<" elementos:" << endl;
            for (int i = 0; i < command_freq.size(); i++) {
                printf("%d %s\n", command_freq[i].first, command_freq[i].second);
            }
            goto end;
        }


        //Run program
        executeProgram(cmd);
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
  char curDir[1024];
  char curUsr[64];
  char curHst[64];
  do{
  // Vuelve a preguntar mientras
    //printf(">>> ");
    // Se aprete Enter
    getcwd(curDir, sizeof(curDir));
    getlogin_r(curUsr, sizeof(curUsr));
    gethostname(curHst, sizeof(curHst));
    cout << getpid() <<" \033[1;31m"<<curUsr<<"@"<<curHst<<"\033[0m:\033[1;36m~"<<get_current_dir_name()<< "\033[0m > ";
    fgets(line, 100, stdin);
  }while(line[0] == '\n');


  //NEEDLESS SPACES START

  line[strlen(line) - 1] = '\0';



  //NEEDLESS SPACES END


  /*
  for(int i = 0; i < strlen(line); i++){
    if(line[i] == '\n')
      //printf("En pos i: %d", i);
      cout<<"En pos i: "<<i<<endl;
  }*/
  return true;
  //printf("Prompt leyo: %s\n", line);
}
