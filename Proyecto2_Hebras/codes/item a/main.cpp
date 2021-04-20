#include "monitor.h"
#include <stdlib.h>
#include <time.h>


using namespace std;

int N;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

Monitor m = Monitor();

void *Ida(void *arg){

  bool dir = true;

  m.Punto_Entrada(dir);
  m.Punto_Salida();

}

void *Vuelta(void *arg){

  bool dir = false;

  m.Punto_Entrada(dir);
  m.Punto_Salida();
  
}

int main(){

  srand(time(NULL));

  int n_total = rand()%101;

  cout << "Vehiculos totales: " << n_total << endl;

  pthread_t threads[n_total];

  cout << "Ingrese N: ";
  cin >> N;
  m.GetN(N);

  for(int i = 0; i < n_total; i++){
    if(rand()%2)
      pthread_create(&threads[i], NULL, &Ida, NULL);
    else
      pthread_create(&threads[i], NULL, &Vuelta, NULL);
  }

  // Espera a que terminen todas las hebras para terminar el main
  for(int i = 0; i < n_total; i++){
    pthread_join(threads[i], NULL);
  }

  cout << "Main terminado!\n";
  
  return 0;
}
