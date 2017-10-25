#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_COUNT 100000
#define THREADS 8           //Numero de threads

int I = 0;

void protocolo_entrada() {
    asm(
            ".data\n\t"
            "lock:\n\t"                     //Variable estatica lock, que contendra el estado de la seccion critica (ocupado o desocupado)
            ".byte 0\n\t"                   //Se inicializa a 1
            ".text\n\t"
            "_protocolo_de_entrada:\n\t"    //Label de salto para el bucle de espera
            "movb $1, %al\n\t" 	            //Movemos un 1 al registro AL
            "xchgb (lock),%al\n\t"          //Intercambiamos el valor de AL con el de la variable Lock de forma atómica
            "cmp $0, %al\n\t"	              //Comprobamos lo que habia en lock, si habia un 0, está libre y ahora está el uno que hemos movido, por tanto queda ocupada
            "jne _protocolo_de_entrada\n\t" //Si lock valia 1, está ocupada y volvemos a comprobar
        );
}

void seccion_critica(){
    I = I + 1;
}

void protocolo_salida() {
    asm("movb $0, (lock)");           );      //Al salir de la s.c, volvemos a dejarla libre moviendo un 0 a Lock
}

void *codigo_del_hilo (void *id){
  int i = *(int *)id;
  int k;

  for(k=0; k<MAX_COUNT; k++){
      protocolo_entrada();
      seccion_critica();
      protocolo_salida();
  }
  pthread_exit (id);
}

// Inicializa el vector de ID's en funcion del número de threads que hay
void init(int *id) {

    for (int i = 0; i < THREADS; i++) {
        id[i] = i;
    }
}

int main(){
  int h;
  pthread_t hilos[THREADS];
  int id[THREADS];
  init(id);
  int error;
  int *salida;

  for(h=0; h<THREADS; h++){
    error = pthread_create( &hilos[h], NULL, codigo_del_hilo, &id[h]);

    if (error){
      fprintf (stderr, "Error: %d: %s\n", error, strerror (error));
      exit(-1);
    }
  }

  for(h=0; h<THREADS; h++){
    error = pthread_join(hilos[h], (void **)&salida);
    if (error)
      fprintf (stderr, "Error: %d: %s\n", error, strerror (error));
    else
      printf ("Hilo %d terminado\n", *salida);
  }

  printf ("I = %d   Esperado = %d \n", I, MAX_COUNT * THREADS);
}
