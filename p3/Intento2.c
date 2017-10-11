/**
$hilos
  Compilación: cc -o hilos hilos.c -lpthread
**/
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int I = 0;
unsigned short estados[2] = {0,0};

void *codigo_del_hilo (void *id){
  int i = *(int *)id;
  int j = (i == 0)? 1 : 0;
  int k;
  for(k=0; k<1000; k++){
  // protocolo de entrada
    while(estados[j] == 1);
    estados[i] = 1;

    // Sección crítica
    I = (I + 1)%10;
    printf("En hilo %d, I=%d\n", i,I);
    // protocolo salida
    estados[i] = 0;
    // Resto
  }
  pthread_exit (id);
}

int main(){
  int h;
  pthread_t hilos[2];
  int id[2]={0,1};
  int error;
  int *salida;

  for(h=0; h<2; h++){
    error = pthread_create( &hilos[h], NULL, codigo_del_hilo, &id[h]);

    if (error){
      fprintf (stderr, "Error: %d: %s\n", error, strerror (error));
      exit(-1);
    }
  }

  for(h=0; h<2; h++){
    error = pthread_join(hilos[h], (void **)&salida);
    if (error)
      fprintf (stderr, "Error: %d: %s\n", error, strerror (error));
    else
      printf ("Hilo %d terminado\n", *salida);
  }
}
