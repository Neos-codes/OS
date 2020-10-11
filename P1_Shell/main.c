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

int stringParsing(char **cmd, char *line, int *numPipes);

bool promptLine(char *line);

void executeProgram(char *cmd[100], int words, bool runBg, vector<int> &activeProcesses, int numPipes);

int update_freq (char **cmd, int wordsInCmd, vector <pair<int, char *> > &command_freq);

void createZombies(char *cmd, vector <pid_t> &zombie_list);

void the_purge(vector <pid_t> &zombie_list);

int main(){

  vector <pair<int, char *> > command_freq;
  command_freq.push_back(make_pair(0, strdup("mostrarFrec")));
  vector <pid_t> zombie_list;

	char line[100];
	char *cmd[100];
	vector<int> activeProcesses;
	bool running = true;
	int pidDummy;
	int waitDummy;
	int status;
	bool runBg;
	int wordsInCmd;
	int numPipes=0;




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
		if (numPipes != 0)
		{
			//cout<<"numpipes era "<<numPipes<<" y ahora es 0\n";
			numPipes=0;
		}
		wordsInCmd = stringParsing(cmd, line, &numPipes);
		//cout<<"numPipes = "<<numPipes<<endl;
		for (int i = 0; i < wordsInCmd; ++i)
		{
			cout<<cmd[i]<<endl;
		}

		top:
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

    if (!strcmp(cmd[0], "arise")){
      createZombies(cmd[1], zombie_list);
      continue;
    }

    if (!strcmp(cmd[0], "purge")){
      the_purge(zombie_list);
      continue;
    }

		if (!strcmp(cmd[0], "mostrarFrec")){

			vector <pair<int, char *> > aux = command_freq;
			sort(aux.begin(), aux.end());
			reverse(aux.begin(), aux.end());
			int k;
			if (cmd[1] == NULL) {
					k = 5;
			} else {
					k = atoi(cmd[1]);
			}
			cout << "Los " << k << " comandos más utlilzados son:" << endl;
			for (int i = 0; i < k && i < command_freq.size(); i++) {
					cout << "(" << i+1 << ")" << " " << aux[i].second << endl;
					// printf("(%d) %s\n", count, aux[i].second);
			}
			cout << "Desea ejecutar alguno de estos? (1/2/.../k o 'n' para declinar)" << endl;
			// char c[4];
			string c;
			readC:
			cin >> c;
			getchar();
			if (!c.compare("n")) continue;
			for (int i = 0; i < c.size(); i++) {
					if (c.c_str()[i] < '0' || c.c_str()[i] > '9') {
							cout << "Indice invalido" << endl;
							goto readC;
					}
			}
			int num;
			num = atoi(c.c_str());
			if (num > k || num < 1) {
					cout << "Indice invalido" << endl;
					goto readC;
			}
			numPipes=0;
			wordsInCmd = stringParsing(cmd, aux[num-1].second, &numPipes);
			cout << "cant de argumentos del comando es: " << wordsInCmd << endl;
			goto top;
		}

    if (!strcmp(cmd[0], "help")){
      cout<<"Welcome to CFF shell, it includes support for the following built-in commands"<<endl;
      cout<<"-cd [directory]\n-mostrarFrec [n(default: 5)]\n-arise [n(default: 10)]\n-help\n-exit\n";
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

		//Run program
		if(wordsInCmd > 0){
			executeProgram(cmd, wordsInCmd, runBg, activeProcesses, numPipes);
		}
	}

	return 0;

}


int stringParsing(char **cmd, char *line, int *numPipes){

	int nwords = 0, count = 0;
	char *temp;

	bool inSpace = true;
	while(line[count] != '\0'){      // Contamos palabras
		if(line[count] == '|'){
			*numPipes+=1;
			//cout<<"numPipes se le suma y es = "<<*numPipes<<endl;
		}
		if(line[count] == ' '){
			inSpace = true;
		}
		else if(inSpace){
			nwords++;
			inSpace = false;
		}
		count++;
	}//EndWhile

	// Pedimos espacio para punteros de las palabras con malloc

	count = 0;
	//cout<<"problem?\n";
	while(1){
		cmd[count] = (temp = strsep(&line, " "));

		if (cmd[count] == NULL){
			break;
		}
		if (strlen(cmd[count]) == 0){
			continue;
		}
		count++;
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
		if (!strcmp(cmd[0], "mostrarFrec")) {
			command_freq[0].first++;
			return 0;
		}

		char str[100];
		strcpy(str, cmd[0]);


		for (int i = 1; i < wordsInCmd; i++) {
			strcat(str, " ");
			strcat(str, cmd[i]);
		}
		// cout << "el comando reconstruido y siendo agregado a command_freq es: "<< str << endl;
		for (int i = 0; i < command_freq.size(); i++) {
			if (strcmp(str, command_freq[i].second) == 0) {
				command_freq[i].first++;

				// cout << "lista de frecuencias de los comandos:" << endl; //debug
				// for (int i = 0; i < command_freq.size(); i++) {
				//     cout << command_freq[i]. first << " " << command_freq[i].second << endl;
				// }

				return 0;
			}
		}
		pair<int, char *> aux;
		aux.first = 1;
		aux.second = strdup(str);
		command_freq.push_back(aux);
		// cout << "comando nuevo, agregado a la lista." << endl;

		// cout << "lista de frecuencias de los comandos:" << endl; //debug
		// for (int i = 0; i < command_freq.size(); i++) {
		//     cout << command_freq[i]. first << " " << command_freq[i].second << endl;
		// }

		return 0;
}

void executeProgram(char *cmd[100], int words, bool runBg, vector<int> &activeProcesses, int numPipes){
	//cout<<"Ejecuto fork()\n";
	pid_t pid;
	int status;
	if(numPipes == 0){
		//EJECUCION SIN PIPES NO ES AFECTADA
		pid = fork();
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
				waitpid(pid, &status, 0);
			}
			else{
				//Insertamos proceso en la lista de procesos del programa
				activeProcesses.push_back(pid);
				cout<<"["<<pid<<"] in background"<<endl;
			}
		}
	}
	else{
		//Hay que usar PIPES
		cout<<"numPipes no es 0, ejecutamos fork usando pipes\n";
		cout<<"numPipes en executeProgram: "<<numPipes<<endl;

		//aux es usado para ir guardando a medida que uno hace pipes
		char *aux[100];

		//offset registra offset en el cmd para ir copiando a aux
		//ejemplo
		//cmd
		//ls -l | ps -ppid 400
		//        ^
		//        offset
		int offset=0;

		//cantidad de palabras para cada cmd de pipe
		int wordsInPipe;

		//pipe
		int fd[2];

		//backup
		int fdd = 0;

		//:D
		int status;

		while(words > offset){

			wordsInPipe=0;
			for (; offset < words; ++offset){
				if (!strcmp(cmd[offset], "|")){
					offset++;
					break;
				}
				else{
					aux[wordsInPipe] = cmd[offset];
					wordsInPipe++;
				}
			}
			aux[wordsInPipe] = NULL;
			cout<<"En aux: \n";
			for (int i = 0; i < wordsInPipe; ++i)
			{
				cout<<aux[i]<<endl;
			}
			cout<<"OFFSET "<<offset<<endl;
			cout<<"WORDS "<<words<<endl;
			//system("echo FUNCIONA CTM");
			pipe(fd);

			pid = fork();

			if(pid < 0){
				cout<<"Error al crear hijo!\n";
				exit(0);
			}
			else if (pid == 0) {

				dup2(fdd,0);
				//Primer hijo lee de entrada estandar
				//Subsiguientes hijos leen del pipe por fdd = fd[0];

				if(words != offset){
					dup2(fd[1],1);
					//Si no estamos en el final
					//El hijo escribe en PIPE
				}
				close(fd[0]);
				//NO LEE DEL PIPE

				execvp(aux[0], aux);
				cout<<"No se reconoce el comando ";
				for (int i = 0; i < wordsInPipe; ++i){
					cout<<aux[i]<<" ";
				}
				cout<<endl;
				exit(1);
			}
			else{

				wait(NULL);
				close(fd[1]);
				fdd = fd[0];

			}

		}

	}

}



void createZombies(char *cmd, vector <pid_t> &zombie_list) {
    int n;
    if (cmd == NULL) {
        n = 10;
    } else {
        n = atoi(cmd);
    }

    cout << "spawning " << n << " zombies\n";
    pid_t pid;
    for (int i = 0; i < n; i++) {
        pid = fork();
        if (pid < 0) {
            cout << "error al crear hijo\n";
        } else if (pid == 0) {
            exit(0);
        } else {
            cout << "Proceso " << getpid() << " a creado zombie " << pid << "\n";
            zombie_list.push_back(pid);
        }
    }
}

void the_purge(vector <pid_t> &zombie_list) {
    int status;
    for (int i = 0; i < zombie_list.size(); i++) {
        waitpid(zombie_list[i], &status, 0);
    }
}
