#include "comida.h"

void setup(comida_t *comida) {
    for(int i = 0; i < PHILOSOPHERS; ++i) {
        philosopher_t p = PHILOSOPHER_INIT (i);           //inicializacion del filosofos
        pthread_cond_t cond = PTHREAD_COND_INITIALIZER;   //inicializacion del monitor

        comida->philosophers[i] = p;
        comida->picks[i] = 2;
        comida->canEat[i] = cond;
    }
}

void pick(uint8_t id, comida_t *comida) {

    pthread_mutex_lock(&comida->lock);                    //mutex para que solo un filosofo coja palillos

    while(comida->picks[id] != 2)
        pthread_cond_wait(&comida->canEat[id], &comida->lock);  //esperamos a tener dos palillos

    comida->picks[comida->philosophers[id].left] -= 1;          //actualizamos los palillos de los filosofos adyacentes
    comida->picks[comida->philosophers[id].right] -= 1;

    pthread_mutex_unlock(&comida->lock);                  //liberamos el mutex
}

void release(uint8_t id, comida_t *comida) {
    pthread_mutex_lock(&comida->lock);                  //mutex para que solo un filosofo de sus palillos

    comida->picks[comida->philosophers[id].left] += 1;  //le damos los palillos a los filosofos adyacentes
    comida->picks[comida->philosophers[id].right] += 1;

    if(comida->picks[comida->philosophers[id].left] == 2)                        //|
        pthread_cond_signal(&comida->canEat[comida->philosophers[id].left]);     //|
                                                                                 //|Notificamos a los filosofos adyacentes cuando tengan los dos palillos disponibles
    if(comida->picks[comida->philosophers[id].right] == 2)                       //|
        pthread_cond_signal(&comida->canEat[comida->philosophers[id].right]);    //|

    pthread_mutex_unlock(&comida->lock);
}

void eat(uint8_t id) {
    printf("Filosofo %d: empieza a comer\n", id);
    usleep(100*1000);
    printf("Filosofo %d: acaba de comer\n", id);
}
