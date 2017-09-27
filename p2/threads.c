/**
$hilos
  Compilación: cc -o hilos hilos.c -lpthread
**/
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_HILOS 5

int I = 0;

void *codigo_del_hilo (void *id){
   int i;
   for( i = 0; i < 50; i++)
      printf("Hilo %d: i = %d, I = %d\n", *(int *)id, i, I++);            //|imprimimos y casteamos el void * como un *(int *), es decir, el contenido de
                                                                          //|el puntero id casteado como un int
   pthread_exit (id);                                                     //acabamos el thread
}

int main(){
   int h;
   pthread_t hilos[NUM_HILOS];                                            //declaración de los hilos
   int id[NUM_HILOS] = {1,2,3,4,5};                                       //entero identificador de los hilos
   int error;
   int *salida;                                                           //puntero a entero para el valor que devuelve el hilo

   for(h = 0; h < NUM_HILOS; h++){
      error = pthread_create( &hilos[h], NULL, codigo_del_hilo, &id[h]);  //|Creamos los hilos, pasamos como parámetros: la direccion de memoria del hilo,
                                                                          //|NULL hace referencia a una estructura con parametros de ejecución del hilo
                                                                          //|codigo_del_hilo es el nombre de la funcion a ejecutar por el hilo
                                                                          //|&id[h] es el parametro (direccion de memoria) que se le pasa a la funcion,
                                                                          //|y que esta lo recibe como un void *
      if (error){
        fprintf (stderr, "Error: %d: %s\n", error, strerror (error));
        exit(-1);
      }
   }

   for(h =0; h < NUM_HILOS; h++){
      error = pthread_join(hilos[h], (void **)&salida);                   //|esperamos a que el thread h acabe su ejecución, como segundo parametro tenemos el void *
                                                                          //|que en este caso es un entero y que para acceder a el hay que hacer doble puntero a void ya que
                                                                          //|&salida es la direccion de memoria del puntero que apunta al puntero id que a su vez apunta al valor
                                                                          //|id que nos interesa
      if (error)
         fprintf (stderr, "Error: %d: %s\n", error, strerror (error));
      else
         printf ("Hilo %d terminado\n", *salida);                         //|se accede al valor id con el contenido de el puntero de salida
   }
}
