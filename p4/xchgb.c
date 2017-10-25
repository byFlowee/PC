#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_COUNT 100000
#define THREADS 8

int I = 0;

void protocolo_entrada() {
    asm(
            ".data\n\t"	/* Declaro la variable estatica lock (solo vale para hilos)*/
            "lock:\n\t"
            ".byte 0\n\t"  /* la inicializo a 0 (seccion critica libre) */
            ".text\n\t"	/* comienzan las instrucciones */
            "_protocolo_de_entrada:\n\t"   /* etiqueta al principio de la espera ocupada */
            "movb $1, %al\n\t" 	 /* muevo 1 al registro AL */
            "xchgb (lock),%al\n\t"  /* intercambio el valor de lock con 1 de manera atomica*/
            "cmp $0, %al\n\t"	 /* comparo lo que habia en lock con 0 */
            "jne _protocolo_de_entrada\n\t"  /* si lock era 0, la seccion critica estaba libre */
    					  /* y salgo de la espera ocupada, ahora lock = 1,  */
    					  /* si lock era 1, sigue siendo 1 y vuelvo a comprobar */
        );
}

void seccion_critica(){
    I = I + 1;
}

void protocolo_salida() {
    asm("movb $0, (lock)");
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
