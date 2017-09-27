#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

#define NUM_PROCESOS 2

void print(int d) {
  printf("%d\n", d);
  exit(d);
}

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
    else if (pid == 0) {
      print(i);
    }
  }  
}
