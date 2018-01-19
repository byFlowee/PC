#include <sys/types.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define AVIONES 25
#define K_HANGARES 10

pthread_mutex_t pista = PTHREAD_MUTEX_INITIALIZER;      //Mutex que controla el acceso a la pista
sem_t hangar;                                           //Semaforo que controla los hangares disponibles (inicializado a K_HANGARES)

void aterrizar(uint8_t id) {
    sem_wait(&hangar);              //reservamos el hangar antes de intentar reservar la pista
    printf("El avion %d reserva hangar\n", id);
    pthread_mutex_lock(&pista);     //con el hangar reservado, intentamos obtener la pista

    usleep(250*1000);
    printf("El avion %d aterriza\n", id);

    pthread_mutex_unlock(&pista);   //liberamos la pista una vez aterrizamos
}

void cargar(uint8_t id) {
    usleep(100*1000);
    printf("El avion %d carga\n", id);
}

void descargar(uint8_t id) {
    usleep(100*1000);
    printf("El avion %d descarga\n", id);

    pthread_mutex_lock(&pista); //tratamos de obtener la pista para despegar
    sem_post(&hangar);          //una vez obtenemos la pista  podemos liberar el hangar y despegar
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
        usleep((rand() % 200 + 100)*1000);  //esperamos un tiempo aleatorio entre 100 y 200 milisegundos para la llegada de los aviones
        pthread_create(&av[i], NULL, run, (void *)&i);
    }

    for (i = 0; i < AVIONES; ++i) {
        pthread_join(av[i], NULL);
    }

    return 0;
}
