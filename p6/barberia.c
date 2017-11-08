#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_CLIENTES 20
#define PLAZAS_SOFA 4
#define NUM_BARBEROS 3
#define NUM_CLIENTES 50

int counter = -1;
int readers = 0;                                        //Cantidad de lectores accediendo al recurso

struct tdata {
    int tid;
};

pthread_mutex_t terminado = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t dejar_silla = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cliente_listo = PTHREAD_MUTEX_INITIALIZER;

sem_t max_clientes;
sem_t sofa;
sem_t silla_barbero;
sem_t coord_barbero;
sem_t pago;
sem_t recibo;

void *cajero(void *ptr) {
    while(true) {
        sem_wait(&pago);
        sem_wait(&coord_barbero);
        sem_post(&coord_barbero);
        sem_post(&recibo);
    }
}

void *barbero(void *ptr) {
    while(true) {
        pthread_mutex_lock(&cliente_listo);
        sem_wait(&coord_barbero);
        sem_post(&coord_barbero);
        pthread_mutex_unlock(&terminado);
        pthread_mutex_lock(&dejar_silla);
        sem_post(&silla_barbero);
    }
}

void *client(void *ptr) {
    sem_wait(&max_clientes);

    printf("Cliente entra en la barberia\n");

    sem_wait(&sofa);

    printf("Cliente se sienta en el sofa\n");

    sem_wait(&silla_barbero);

    printf("Cliente se sienta en la silla del barbero\n");

    sem_post(&sofa);
    pthread_mutex_unlock(&cliente_listo);
    pthread_mutex_lock(&terminado);
    pthread_mutex_unlock(&dejar_silla);
    sem_post(&pago);
    sem_wait(&recibo);
    sem_post(&max_clientes);
}

int main (int argc, char *argv[]) {
    pthread_t th_clientes[NUM_WRITERS+NUM_READERS];
    pthread_t th_barberos[NUM_WRITERS+NUM_READERS];
    int rc, i;
    struct tdata id[NUM_WRITERS+NUM_READERS];

    sem_init(&max_clientes, 0, MAX_CLIENTES);
    sem_init(&sofa, 0, PLAZAS_SOFA);
    sem_init(&silla_barbero, 0, NUM_BARBEROS);
    sem_init(&coord_barbero, 0, NUM_BARBEROS);
    sem_init(&pago, 0, 0);
    sem_init(&recibo, 0, 0);

    pthread_mutex_lock(&cliente_listo);

    for(i=0; i<NUM_BARBEROS; i++){
        id[i].tid = i;
        rc = pthread_create(&th_barberos[i], NULL, barbero, (void *) &id[i]);
    }

    for(i=0; i<NUM_CLIENTES; i++){
        id[i].tid = i;
        rc = pthread_create(&th_clientes[i], NULL, client, (void *) &id[i]);
    }

    for(i=0; i<NUM_READERS + NUM_WRITERS; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}
