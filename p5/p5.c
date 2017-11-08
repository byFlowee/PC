#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


#define NUM_READERS 10
#define NUM_WRITERS 5

int counter = 0;
int readers = 0;

struct tdata {
    int tid;
};

/* POSIX Thread mutex */
pthread_mutex_t writer = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;
/* Don't confuse it with POSIX Semaphore */

void reader_lock() {
    pthread_mutex_lock(&mx);                            //Bloqueamos mx para asegurar que se accede a la variable readers en exclusion mutua
    readers++;                                          //Añadimos un lector al contador de lectores
    if (readers == 1) pthread_mutex_lock(&writer);      //El primer lector en entrar bloquea a los escritores ya que no pueden escribir mientras haya lectores
    pthread_mutex_unlock(&mx);                          //Desbloqueamos mx al terminar para que pasen otros lectores
}

void reader_unlock() {
    pthread_mutex_lock(&mx);                            //Bloqueamos mx para asegurar que se accede a la variable readers en exclusion mutua
    readers--;                                          //Quitamos un lector del contador de lectores
    if (readers == 0) pthread_mutex_unlock(&writer);    //Si no queda ningun lector al salir, desbloqueamos a los escritores
    pthread_mutex_unlock(&mx);                          //Desbloqueamos mx al terminar
}

//Bloquea la entrada a los escritores
void writer_lock() {
    pthread_mutex_lock(&writer);
}

//Desbloquea a los escritores
void writer_unlock() {
    pthread_mutex_unlock(&writer);
}

//Hilos de lectores
void *readerThread(void *ptr) {
    int c;
    for (int i = 0; i < 5; i++) {   //Cada lector lee 5 veces la variable
        usleep(100);

        reader_lock();

        c = counter;
        printf("El lector %d ha leido un valor de %d\n", ((struct tdata *) ptr)->tid, c);

        reader_unlock();
    }
}

//Hilos de escritores
void *writerThread(void *ptr) {
    for (int i = 0; i < 5; i++) {   //Cada escritor actualiza 5 veces la variable
        usleep(100);

        writer_lock();              //Queremos escribir, se bloquea hasta que un lector nos de señal

        counter += 1;
        printf("El escritor %d ha actualizado el recurso\n", ((struct tdata *) ptr)->tid);

        writer_unlock();
    }
}

int main (int argc, char *argv[]) {
    pthread_t threads[NUM_WRITERS+NUM_READERS];
    int rc, i;
    struct tdata id[NUM_WRITERS+NUM_READERS];

    for(i=NUM_READERS; i<NUM_WRITERS + NUM_READERS; i++){   //Hilos de escritores
        id[i].tid = i;
        rc = pthread_create(&threads[i], NULL, writerThread, (void *) &id[i]);
    }

    for(i=0; i<NUM_READERS; i++){                           //Hilos de lectores
        id[i].tid = i;
        rc = pthread_create(&threads[i], NULL, readerThread, (void *) &id[i]);
    }

    for(i=0; i<NUM_READERS + NUM_WRITERS; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}
