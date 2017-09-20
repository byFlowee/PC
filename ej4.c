#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

#define NUM_PROCESOS 3
#define NUM_CHAR 5

void print(char c) {
  int i;
  for (i = 0; i < 5; i++) {
    printf("%c\n", c);
  }
  exit(c);
}

int main() {
  int i;
  int id;
  int pid;

  for (i = 0; i<NUM_PROCESOS; i++) {
    pid = fork();
    if (pid == -1) {
      perror("Error al crear el proceso");
      exit(-1);
    }
    else if (pid == 0) {
      print(i+65); //Valor de i + Codigo ascii de 'A'
    }
  }
  for (i = 0; i<NUM_PROCESOS; i++) {
    wait(&id);
  }
}
