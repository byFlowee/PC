#include "comida.h"

comida_t comida;

void *run(void *arg) {
    uint8_t id = *((uint8_t *)arg);
    int i;

    for (i = 0; i < EAT_COUNT; ++i) {
        usleep(250*1000);
        pick(id, &comida);
        eat(id);
        release(id, &comida);
    }
}

int main() {
    pthread_t ph[PHILOSOPHERS];
    setup(&comida);             //inicializamos todos los filosofos

    uint8_t i;

    //creamos los hilos
    for (i = 0; i < PHILOSOPHERS; ++i) {
        pthread_create(&ph[i], NULL, run, (void *)&i);
    }

    //esperamos a que acaben
    for (i = 0; i < PHILOSOPHERS; ++i) {
        pthread_join(ph[i], NULL);
    }

    return 0;
}
