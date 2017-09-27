/**
$hilos
  Compilación: cc -o hilos hilos.c -lpthread
**/
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_HILOS 3

int I = 0;

typedef struct {
  char c;
}Caracter ;

/*2. Realizar un programa que contenga una función
imprimir
que imprima un
carácter cualquiera 50 veces. A la función se le debe pasar como parámetro
una estructura en la que debe ir encapsulado el carácter. En el programa
principal debemos crear 3 hilos concurrentes que impriman una `A`, una `B` y
una `C` respectivamente. */


void *codigo_del_hilo (void *id){
   int i;
   Caracter ch;
   ch = *(Caracter *)id;
   for( i = 0; i < 5; i++)
      printf("%c\n", ch.c);
   pthread_exit (id);
}

int main(){
   int h;
   pthread_t hilos[NUM_HILOS];

   Caracter a;
   a.c = 'A';
   Caracter b;
   b.c = 'B';
   Caracter c;
   c.c = 'C';

   Caracter id[NUM_HILOS] = {a,b,c};
   int error;
   Caracter *salida;

   for(h = 0; h < NUM_HILOS; h++){
      error = pthread_create( &hilos[h], NULL, codigo_del_hilo, &id[h]);
      if (error){
        fprintf (stderr, "Error: %d: %s\n", error, strerror (error));
        exit(-1);
      }
   }

   for(h =0; h < NUM_HILOS; h++){
      error = pthread_join(hilos[h], (void **)&salida);
      if (error)
         fprintf (stderr, "Error: %d: %s\n", error, strerror (error));
      else
         printf ("Hilo %c terminado\n", (*salida).c);
   }
}
