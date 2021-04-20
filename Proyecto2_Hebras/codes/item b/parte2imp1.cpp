#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main(){

  //sudo perf stat -e cycles,instructions,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores,LLC-load-misses,LLC-store-misses,dTLB-load-misses,dTLB-store-misses ./a.out

  srand(time(NULL));
  
  //int n;
  int **A, **B, **C, **Bt;

  int n = 2048;
  // Leer n
  //cin >> n;

  // Crear A, B y D
  A = (int **)malloc(n * sizeof(int *));
  B = (int **)malloc(n * sizeof(int *));
  Bt = (int **)malloc(n * sizeof(int *));
  C = (int **)malloc(n * sizeof(int *));

  for(int i = 0; i < n; i++){
    A[i] = (int *)malloc(n * sizeof(int));
    B[i] = (int *)malloc(n * sizeof(int));
    Bt[i] = (int *)malloc(n * sizeof(int));
    C[i] = (int *)malloc(n * sizeof(int));
  }

  // Llenar A y B

  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      A[i][j] = rand()%11;
      B[i][j] = rand()%11;
    }
  }

  // Imprimir A y B
  /*cout << "A" << endl;
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      cout << A[i][j] << " ";
    }
    cout << endl;
  }

  cout << "B" << endl;
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      cout << B[i][j] << " ";
    }
    cout << endl;
  }*/

  //llenar C con 0s
  for (int i = 0; i < n; ++i){
    for (int j = 0; j < n; ++j){
      C[i][j] = 0;
    }
  }

  //Transponemos B
  for (int i = 0; i < n; ++i){
    for (int j = 0; j < n; ++j){
      Bt[j][i] = B[i][j];
    }
  }

  cerr <<"llego aca"<<endl;

  /*cout << "Bt" << endl;
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      cout << Bt[i][j] << " ";
    }
    cout << endl;
  }*/

  // Multiplicar A y Bt por fila
  cout << "Mutliplicacion con transpuesta" << endl;

  int aux = 0, i, j ,k;
  for (i = 0; i < n; ++i){
    for (j = 0; j < n; ++j){
      aux = 0;
      for (k = 0; k < n; ++k){
        aux += A[i][k] * Bt[j][k];
      }
      C[i][j] = aux;
    }
  }

  cout << "Done" << endl;

  // Imprimir C
  /*for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      cout << C[i][j] << " ";
    }
    cout << endl;
  }*/

  return 0;
}
