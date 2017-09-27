/***
$procesos
***/
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

#define NUM_PROCESOS 5

int I = 0;

void codigo_del_proceso (int id){
	int i;
	for(i = 0; i < 50; i++)
		printf("Proceso%d: i = %d, I = %d\n", id, i, I++);	//imprime el id del proceso y el valor de la variable global actualizado
	exit(id); // el id se almacena en los bits 8 al 15 antes de devolverlo al padre
}

int main(){
	int p;
	int id [NUM_PROCESOS] = {1,2,3,4,5};				//array de identificadores del proceso
	int pid;
	int salida;

	for(p = 0; p < NUM_PROCESOS; p++){
		pid= fork();									 						//se crea un nuevo proceso identico al proceso padre y continua la ejecución
		if(pid == -1){														//si el pid es -1 error
			perror("Error al crear un proceso: ");
			exit(-1);
		}
		else if (pid == 0) 												// Codigo del hijo, fork() devuelve al padre el pid de su hijo y 0 al hijo
			codigo_del_proceso(id[p]);							//si el pid es 0, estamos en el proceso hijo y se llama a la función con el identificador como parametro
	}

	// Codigo del padre
	for (p = 0; p < NUM_PROCESOS; p++){					//para cada proceso creado
		pid = wait (&salida);											//se espera que termine su ejecución y se recoge el valor en la direccion de memoria del parametro
		printf("Proceso %d con id = %x terminado\n", pid, salida >> 8);		//se imprime el valor al que apunta salida desplazado 8 bits
	}																																		//ya que el valor almacenado va del 8 al 15
}
