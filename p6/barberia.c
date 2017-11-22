#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX_CLIENTES 20
#define PLAZAS_SOFA 4
#define NUM_BARBEROS 3
#define NUM_CLIENTES 20

int fin = 1;
int currentClient = 0;

struct tdata {
    int tid;
};

pthread_mutex_t terminado = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t dejar_silla = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cliente_listo = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t pago = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t recibo = PTHREAD_MUTEX_INITIALIZER;

sem_t max_clientes;
sem_t sofa;
sem_t silla_barbero;
sem_t coord_barbero;

/*wait(pago)
  wait(coord)
  cobrar()
  signal(coord)
  signal(recibo)
*/

void *cajero(void *ptr) {
    int id = ((struct tdata *) ptr)->tid - NUM_BARBEROS;
    while(1) {
        pthread_mutex_lock(&pago);
        sem_wait(&coord_barbero);

        printf("El barbero %d cobra\n", id);
        usleep(500*1000);

        sem_post(&coord_barbero);
        pthread_mutex_unlock(&recibo);
    }
}

/*wait(cliente_listo)
  wait(coord)
  cortar_pelo()
  signal(coord)
  signal(terminado)
  wait(dejar_silla_barbero)
  signal(silla_barbero)
*/

void *barbero(void *ptr) {
    int id = ((struct tdata *) ptr)->tid;
    while(1) {
        pthread_mutex_lock(&cliente_listo);
        sem_wait(&coord_barbero);

        printf("El barbero %d corta el pelo\n", id);
        usleep(500*1000);

        sem_post(&coord_barbero);
        pthread_mutex_unlock(&terminado);
        pthread_mutex_lock(&dejar_silla);
        sem_post(&silla_barbero);
    }
}

/*wait(max_capacidad)
  wait(sofa)
  wait(silla_barbero)
  signal(sofa)
  signal(cliente_listo);
  wait(terminado);
  signal(dejar_silla_barbero);
  signal(pago);
  wait(recibo);
  signal(max_capacidad);
*/

void *client(void *ptr) {
    int id = ((struct tdata *) ptr)->tid;
    usleep(100*1000);

    sem_wait(&max_clientes);

    usleep(100*1000);
    currentClient++;
    printf("Cliente %d entra en la barberia, hay %d clientes en la tienda\n", id, currentClient);

    sem_wait(&sofa);

    printf("Cliente %d se sienta en el sofa\n", id);
    usleep(100*1000);

    sem_wait(&silla_barbero);

    printf("Cliente %d se levanta del sofa\n", id);

    sem_post(&sofa);

    printf("Cliente %d se sienta en la silla del barbero\n", id);
    usleep(100*1000);

    pthread_mutex_unlock(&cliente_listo);
    pthread_mutex_lock(&terminado);

    printf("Cliente %d se levanta de la silla del barbero\n", id);
    usleep(100*1000);

    pthread_mutex_unlock(&dejar_silla);
    pthread_mutex_unlock(&pago);
    pthread_mutex_lock(&recibo);

    printf("Cliente %d sale de la barberia, quedan %d clientes\n", id, --currentClient);

    sem_post(&max_clientes);
}

int main (int argc, char *argv[]) {
    pthread_t th_clientes[NUM_CLIENTES];
    pthread_t th_barberos[NUM_BARBEROS*2];
    int rc, i;
    struct tdata id[NUM_BARBEROS*2];
    struct tdata id2[NUM_CLIENTES];

    sem_init(&max_clientes, 0, MAX_CLIENTES);
    sem_init(&sofa, 0, PLAZAS_SOFA);
    sem_init(&silla_barbero, 0, NUM_BARBEROS);
    sem_init(&coord_barbero, 0, NUM_BARBEROS);

    pthread_mutex_lock(&cliente_listo);
    pthread_mutex_lock(&pago);
    pthread_mutex_lock(&recibo);

    for(i=0; i<NUM_CLIENTES; i++){
        id2[i].tid = i;
        rc = pthread_create(&th_clientes[i], NULL, client, (void *) &id2[i]);
    }

    for(i=0; i<NUM_BARBEROS; i++){
        id[i].tid = i;
        id[NUM_BARBEROS + i].tid = NUM_BARBEROS + i;
        pthread_create(&th_barberos[i], NULL, barbero, (void *) &id[i]);
        pthread_create(&th_barberos[NUM_BARBEROS + i], NULL, cajero, (void *) &id[NUM_BARBEROS + i]);
    }

    for(i=0; i<NUM_CLIENTES; i++){
        pthread_join(th_clientes[i], NULL);
    }

    return 0;
}
