#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


using namespace std;


int main(){

  srand(time(NULL));
  
  //int n;
  int **A, **B, **C, **D, **E;

  int n = 2048;
  // Leer n
  //cin >> n;

  // Crear A, B y D
  A = (int **)malloc(n * sizeof(int *));
  B = (int **)malloc(n * sizeof(int *));
  C = (int **)malloc(n * sizeof(int *));

  for(int i = 0; i < n; i++){
    A[i] = (int *)malloc(n * sizeof(int));
    B[i] = (int *)malloc(n * sizeof(int));
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

  // Multiplicar A y B
  cout << "Mutliplicacion Simple!" << endl;

  for (int i = 0; i < n; ++i){
    for (int j = 0; j < n; ++j){
      for (int k = 0; k < n; ++k){
        C[i][j] += A[i][k] * B[k][j];  
      }
    }
  }


  // Imprimir C
  /*for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      cout << C[i][j] << " ";
    }
    cout << endl;
  }*/
  
  cout << "Done" << endl;


  
  return 0;
}
