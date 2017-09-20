#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_PROCESOS 3

int I = 0;

void codigo_del_proceso(char character){

	int i = 0;

	for (i = 0; i < 5; i++){

		printf("Imprimo: %c\n", character);
	}

	exit(character); // el id se almacena en los bits 8 al 15 antes de devolverlo al padre
}

int main(){

	int p;
	int letters[NUM_PROCESOS] = {'A', 'B', 'C'};
	int pid;
	int salida;

	for (p = 0; p < NUM_PROCESOS; p++){

		pid = fork();

		if (pid == -1){

			perror("Error al crear un proceso: ");
			exit(-1);
		}
		else if (pid == 0) // Codigo del hijo
			codigo_del_proceso(letters[p]);
	}

	// Codigo del padre
	for (p = 0; p < NUM_PROCESOS; p++){

		pid = wait(&salida);
		printf("Proceso %d con id = %x terminado\n", pid, salida >> 8);
	}
}
