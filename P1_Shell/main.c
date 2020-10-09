#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


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
    cout<<"Error al crear hijo!\n";
    exit(0);
  }
  else if(pid == 0){       // Hijo
    execvp(cmd[0], cmd);
    cout<<"Error al ejecutar comando!\n";
    exit(0);
  }
  else{                   // Padre
    wait(NULL);
  }
}

int update_freq (char **cmd, int wordsInCmd, vector <pair<int, char *> > &command_freq);


int main(){

    vector <pair<int, char *> > command_freq;
    char line[100];
    char *cmd[100];
    int wordsInCmd;
    system("clear");
    bool running = promptLine(line);

    while (running){
        wordsInCmd = stringParsing(cmd, line);
        top:
        update_freq (cmd, wordsInCmd, command_freq);

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
        // if (!strcmp(cmd[0], "revisarVector")){ //for debug
        //     cout << "en el vector hay " << command_freq.size() <<" elementos:" << endl;
        //     for (int i = 0; i < command_freq.size(); i++) {
        //         printf("%d %s\n", command_freq[i].first, command_freq[i].second);
        //     }
        //     goto end;
        // }
        if (!strcmp(cmd[0], "mostrarFrec")){
            if (cmd[1] == NULL) {
                cout << "Error. Expecting integer as argument for mostrarFrec" << endl;
                goto end;
            }

            vector <pair<int, char *> > aux = command_freq;
            sort(aux.begin(), aux.end());
            reverse(aux.begin(), aux.end());
            int k = atoi(cmd[1]);
            cout << "Los " << k << " comandos más utlilzados son:" << endl;
            for (int i = 0; i < k && i < command_freq.size(); i++) {
                cout << "(" << i+1 << ")" << " " << aux[i].second << endl;
                // printf("(%d) %s\n", count, aux[i].second);
            }
            cout << "Desea ejecutar alguno de estos? (1/2/.../k o 'n' para declinar)" << endl;
            // char c[4];
            string c;
            cin >> c;
            getchar();
            if (!c.compare("n")) goto end;
            int num;
            num = atoi(c.c_str());
            wordsInCmd = stringParsing(cmd, aux[num-1].second);
            fflush(stdin);
            goto top;

            // goto end;
        }


        //Run program
        executeProgram(cmd);
        // cout << "debug" << endl;
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

  return true;
  //printf("Prompt leyo: %s\n", line);
}

int update_freq (char **cmd, int wordsInCmd, vector <pair<int, char *> > &command_freq) {
    char str[100];
    strcpy(str, cmd[0]);
    for (int i = 1; i < wordsInCmd; i++) {
        strcat(str, " ");
        strcat(str, cmd[i]);
    }
    for (int i = 0; i < command_freq.size(); i++) {
        if (strcmp(str, command_freq[i].second) == 0) {
            command_freq[i].first++;
            return 0;
        }
    }
    pair<int, char *> aux;
    aux.first = 1;
    aux.second = strdup(str);
    command_freq.push_back(aux);
    // printf("%s check!\n", command_freq.back().second);
    return 0;
}
