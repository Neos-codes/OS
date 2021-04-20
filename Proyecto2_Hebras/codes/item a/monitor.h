#include <pthread.h>
#include <iostream>
#include <unistd.h>

using namespace std;

class Monitor{

 public:
  // Atributos
  bool dirm;
  int N;
  int count;
  int n_ida;
  int n_vuelta;
  int n_total;
  
  // Metodos
  Monitor();
  void Punto_Entrada(bool dir);
  void Punto_Salida();
  void GetN(int N);

 private:
  pthread_mutex_t mutex;
  pthread_mutex_t inMovement;
  pthread_cond_t ida;
  pthread_cond_t vuelta;
  
};

Monitor::Monitor(){

  cout << "Inicializando monitor" << endl;
  // Comienzan los de ida
  this -> dirm = true;
  // Inicializar mutex y var de condicion
  this -> ida = PTHREAD_COND_INITIALIZER;
  this -> vuelta = PTHREAD_COND_INITIALIZER;
  this -> mutex = PTHREAD_MUTEX_INITIALIZER;
  this -> inMovement = PTHREAD_MUTEX_INITIALIZER;
  // Inicializar contadores de autos en 0
  this -> n_ida = 0;
  this -> n_vuelta = 0;
  this -> n_total = 0;
}

void Monitor::Punto_Entrada(bool dir){

  pthread_mutex_lock(&mutex);

  // Agregar vehiculo al contador de vehiculos en espera
  if(dir){
    n_ida++;
  }
  else{
    n_vuelta++;
  }

  // Si la direccion del vehiculo no coincide con la del monitor, esperar
  while(dir != dirm){
    cout << "Vehiculo con dir " << dir << " esperando" << endl;
    if(dir){
      // Si no hay ningun vehiculo en la otra dirección esperando, puede pasar
      if(n_vuelta == 0){
	//cout << "n_vuelta es 0, paso de ida libre" << endl;
	dirm = !dirm;
	count = 0;
	continue;
      }

      pthread_cond_wait(&ida, &mutex);
    }
    else{
      // Si no hay ningun vehiculo esperando en la otra dirección, puede pasar
      if(n_ida == 0){
	//cout << "n_ida es 0, paso de vuelta libre" << endl;
	dirm = !dirm;
	count = 0;
	continue;
      }
      pthread_cond_wait(&vuelta, &mutex);
    }
  }

  pthread_mutex_unlock(&mutex);

  // Desde aquí, consideramos que el vehiculo va pasando por la carretera
  // Usamos el mutex "inMovement" para sellar la carretera y no venga nadie de la otra direccion
  // y el mutex "mutex" para no tocar los mismos valores por otra hebra que esta en la fila
  
  pthread_mutex_lock(&inMovement);
  pthread_mutex_lock(&mutex);

 // Aumentar contador de vehiculos que pasan en la direccion
  count++;

  cout << "Vehiculo pasando con dir" << dir << endl;

  // La direccion se debe cambiar aqui para evitar un choque
  if(count == N){
    // Si vamos de ida y hay vehiculos esperando de vuelta, cambiar la direccion
    if(dirm && n_vuelta != 0){
      dirm = !dirm;
      count = 0;
      //cout << "Nueva dir!, ahora es " << dirm << endl;
    }
    // Si vamos de vuelta y hay vehiculos esperando de ida, cambiar la direccion
    else if(!dirm && n_ida != 0){
      dirm = !dirm;
      count = 0;
      //cout << "Nueva dir! ahora es " << dirm << endl;
    }
    // SI vamos en cualquier direccion y al otro lado no hay vehiculos esperando, dejamos pasar N más de la misma direccion
    else{
      cout << "En el lado contrario no hay autos esperando! El flujo mantiene su direccion y reinicia contador" << endl;
      count = 0;
      }
  }  

  // Disminuir contador que cuenta su presencia en la fila
  if(dir){
    n_ida--;
    n_total++;
  }
  else{
    n_vuelta--;
    n_total++;
  }
  
  pthread_mutex_unlock(&mutex);
 
}

void Monitor::Punto_Salida(){

  pthread_mutex_lock(&mutex);
  pthread_mutex_unlock(&inMovement);

  cout << "Vehiculo paso!" << endl;
  cout << "ida: " << n_ida << " vuelta: " << n_vuelta << " count: " << count << endl;
  
  if(dirm)
    pthread_cond_signal(&ida);
  else if(!dirm)
    pthread_cond_signal(&vuelta);
  
  pthread_mutex_unlock(&mutex);
  
}  
  

void Monitor::GetN(int n){
  this -> N = n;
  this -> count = 0;
}

