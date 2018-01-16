#include <sys/types.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>

#define AVIONES 25
#define K_HANGARES 10

pthread_mutex_t pista = PTHREAD_MUTEX_INITIALIZER;
sem_t hangar;

void aterrizar(uint8_t id) {
    sem_wait(&hangar);
    printf("El avion %d reserva hangar\n", id);
    pthread_mutex_lock(&pista);

    usleep(250*1000);
    printf("El avion %d aterriza\n", id);

    pthread_mutex_unlock(&pista);
}

void cargar(uint8_t id) {
    usleep(100*1000);
    printf("El avion %d carga\n", id);
}

void descargar(uint8_t id) {
    usleep(100*1000);
    printf("El avion %d descarga\n", id);

    pthread_mutex_lock(&pista);
    sem_post(&hangar);
    printf("El avion %d deja el hangar\n", id);
}

void despegar(uint8_t id) {
    usleep(250*1000);
    printf("El avion %d despega\n", id);

    pthread_mutex_unlock(&pista);
}


void *run(void *arg) {
    uint8_t id = *((uint8_t *)arg);         //Cast a unsigned int de 8 bits

    aterrizar(id);
    cargar(id);
    descargar(id);
    despegar(id);
}

int main() {
    pthread_t av[AVIONES];
    uint8_t i;

    sem_init(&hangar, 0, K_HANGARES);

    for (i = 0; i < AVIONES; ++i) {
        pthread_create(&av[i], NULL, run, (void *)&i);
    }

    for (i = 0; i < AVIONES; ++i) {
        pthread_join(av[i], NULL);
    }

    return 0;
}
