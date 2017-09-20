#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

#define NUM_PROCESOS 2

int main() {
  int i;
  int id;
  int pid;

  for (i = 1; i <= NUM_PROCESOS; i++) {
    pid = fork();
    if (pid == -1) {
      perror("Error al crear el proceso");
      exit(-1);
    }
    //codigo del hijo
    else if (pid == 0) {
      printf("%d\n", i);
      exit(i);                //terminamos el proceso para que el hijo no continue la ejecución
    }
  }
}
