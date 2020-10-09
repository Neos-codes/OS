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

int stringParsing(char **cmd, char *line);

bool promptLine(char *line);

void executeProgram(char *cmd[100], int words, bool runBg, vector<int> &activeProcesses);

int update_freq (char **cmd, int wordsInCmd, vector <pair<int, char *> > &command_freq);


int main(){


  vector <pair<int, char *> > command_freq;
  char line[100];
  char *cmd[100];
  vector<int> activeProcesses;
  bool running = true;
  int pidDummy;
  int waitDummy;
  int status;
  //signal(SIGCHLD, sig_handler);


  bool runBg;
  int wordsInCmd;

  FILE *fp = fopen("mishell.log", "a+");
  if(fp == NULL){
    cout << "Fallo apertura o creacion de mishell.log" << endl;
    return 0;
  }
  fseek(fp, 0, SEEK_END);

  system("clear");
  //Notar que hacer system clear crea un proceso que ejecuta el clear
  //esto activa el signal con sigchild


  while (running){
    
  	promptLine(line);
    wordsInCmd = stringParsing(cmd, line);
    update_freq (cmd, wordsInCmd, command_freq);
    for(int i = 0; i < wordsInCmd; i++){
      fprintf(fp, "%s ", cmd[i]);
    }
    fprintf(fp, "\n");
    fseek(fp, 0, SEEK_END);
    
    //Check background processes
    if (activeProcesses.size() > 0){
  		//cout<<"Lista procesos\n";
    	for (int i = 0; i < activeProcesses.size(); ++i)
    	{
    		//cout<<"PID del proceso "<<activeProcesses[i]<<endl;
	    	pidDummy = activeProcesses[i];
	    	waitDummy = waitpid(pidDummy, &status, WNOHANG);
	    	//cout<<waitDummy<<" WAITPID\n";
	    	if (waitDummy == pidDummy){
	    		//cout<<"Pid "<<pidDummy<<" ya termino, se remueve de la lista\n";
	    		activeProcesses.erase(activeProcesses.begin()+i);
	    		--i;
	    	}
    	}
    }


    //Check if function is built in

    if (!strcmp(cmd[0], "exit")){
      if(activeProcesses.size() > 0){        
        cout<<"Aun hay procesos en ejecucion, esta seguro que desea salir? (Y) (N)\n";
      	for (int i = 0; i < activeProcesses.size(); ++i)
      	{
      	  cout<<"Proceso "<<activeProcesses[i]<<endl;
      	}
      	char c = getchar();
        getchar();
        if(c == 121 || c == 89){
        	//Si escribe Y o y se sale
        	
          //se sale del programa
          running = false;
          continue;
        }
        else{
        	continue;
        }
      }
      cout<<"Exiting\n";
      running = false;
      continue;
    }

    if (!strcmp(cmd[0], "cd")){
      chdir(cmd[1]);
      continue;
    }
    
    if (!strcmp(cmd[0], "mostrarFrec")){
            if (cmd[1] == NULL) {
                cout << "Error. Expecting integer as argument for mostrarFrec" << endl;
                continue;
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
            if (!c.compare("n")) continue;
            int num;
            num = atoi(c.c_str());
            wordsInCmd = stringParsing(cmd, aux[num-1].second);
            fflush(stdin);
            continue;
        }
    }
    if (!strcmp(cmd[0], "help")){
      cout<<"Welcome to CFF shell, it includes support for the following built-in commands"<<endl;
      cout<<"-cd [directory]\n-help\n-exit\n";
      cout<<"-------------------\n";
      continue;
    }

    if (!strcmp(cmd[wordsInCmd-1], "&")){
      //cout<<"Hay que correr en BG\n";
      runBg = true;
    }
    else{
      runBg = false;
    }
    if (!strcmp(cmd[0], "help")){
      cout<<"Welcome to CFF shell, it includes support for the following built-in commands"<<endl;
      cout<<"-cd [directory]\n-help\n-exit\n";
      cout<<"-------------------\n";
      continue;
    }

    if (!strcmp(cmd[wordsInCmd-1], "&")){
      cout<<"Hay que correr en BG\n";
      runBg = true;
    }
    else{
      runBg = false;
    }

    //Run program
    if(wordsInCmd > 0){
      executeProgram(cmd, wordsInCmd, runBg, activeProcesses);
    }
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
    // Se apreto Enter y se muestra el prompt
    getcwd(curDir, sizeof(curDir));
    getlogin_r(curUsr, sizeof(curUsr));
    gethostname(curHst, sizeof(curHst));
    cout<<"["<<getpid()<<"]\033[1;31m"<<curUsr<<"@"<<curHst<<"\033[0m:\033[1;36m~"<<get_current_dir_name()<<" "<<"\033[0m> ";
    fgets(line, 100, stdin);
  }while(line[0] == '\n');
  //<<getpid()
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
void executeProgram(char *cmd[100], int words, bool runBg, vector<int> &activeProcesses){
  //cout<<"Ejecuto fork()\n";
  pid_t pid = fork();
  if(pid < 0){
    //printf("Error al crear hijo!\n");
    cout<<"Error al crear hijo!\n";
    exit(0);
  }
  else if(pid == 0){       // Hijo
    //printf("Soy hijo H: %d mi padre P : %d\n", getpid(), getppid());
    //printf("Hijo creado\n");
    //cout<<"Hijo creado\n";
    //char *cmds[3] = {"ls", "-n", NULL};
    execvp(cmd[0], cmd);

    //Si el comando no se reconoce se imprime el comando ejecutado
    cout<<"No se reconoce el comando ";
    for (int i = 0; i < words; ++i)
    {
      cout<<cmd[i]<<" ";
    }
    cout<<endl;

    //Terminar el hijo
    exit(0);
  }
  else{                   
  	// Padre
    //printf("Soy padre P: %d mi Hijo : %d\n", getpid(), pid);
    if (!runBg){
      wait(NULL);
    }
    else{
    	//Insertamos proceso en la lista de procesos del programa
    	activeProcesses.push_back(pid);
    	cout<<"["<<pid<<"] in background"<<endl;
    }
    
    //
    //exit(0);
  }
}
