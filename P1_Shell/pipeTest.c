#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

  int fd[2];     // Creamos file descriptors,  fd[0] entrada, fd[1] salida
  pipe(fd);     // Creamos el pipe

  pid_t pid = fork();     // Creamos hijo

  if(pid < 0){              // Error al crear hijo
    printf("Error al crear hijo!\n");
    return 0;
  }
  else if(!pid){           // Hijo

    printf("Hijo creado!\n");
    close(fd[0]);          // Cerramos entrada de pipe
    close(1);               // Cerramos salida estandar
    char msg[] = "Hola! Este es un mensaje a traves del pipe!\n";
    
    int size = strlen(msg);               // Guardamos tamaño del mensaje
    printf("El tamano enviado por el hijo es: %d\n", size);
    
    write(fd[1], &size, sizeof(int));   // Enviamos tamaño del mensaje
    write(fd[1], msg, strlen(msg));   // Enviar por salida del pipe
    exit(0);
  }
  else{                       // Padre
    
    printf("Ejecutando Padre!\n");
    close(fd[1]);           // Cerramos salida de pipe
    close(0);                // Cerramos entrada estandar
    char msg[50];        // Para recibir el mensaje enviado por el hijo
    int msgSize;           // Para recibir tamaño del mensaje
    
    read(fd[0], &msgSize, sizeof(int));    // Leemos tamaño de mensaje
    printf("El tamaño recibido por el padre es: %d\n", msgSize);
    
    read(fd[0], msg, sizeof(msg));    // Leemos mensaje  por entrada del pipe
    msg[msgSize] = '\0';
    printf("El mensaje recibido por el pipe fue:\n%s\n", msg);
    exit(0);
  }

  return 0;
}

//
