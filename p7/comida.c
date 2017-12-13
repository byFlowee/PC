#include "comida.h"
#include <stdio.h>

void setup(comida_t *comida) {
    for(int i = 0; i < PHILOSOPHERS; ++i) {
        philosopher_t p = PHILOSOPHER_INIT (i);
        pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
        comida->philosophers[i] = p;
        comida->picks[i] = 2;
        comida->canEat[i] = cond;
    }
}

void pick(uint8_t id, comida_t *comida) {

    pthread_mutex_lock(&comida->lock);

    while(comida->picks[id] != 2) {
        pthread_cond_wait(&comida->canEat[id], &comida->lock);
    }

    comida->picks[comida->philosophers[id]->left] -= 1;
    comida->picks[comida->philosophers[id]->right] -= 1;

    pthread_mutex_unlock(&comida->lock);
}

void release(uint8_t id, comida_t *comida) {
    pthread_mutex_lock(&comida->lock);

    comida->picks[comida->philosophers[id]->left] += 1;
    comida->picks[comida->philosophers[id]->right] += 1;

    if(comida->picks[comida->philosophers[id]->left] == 2)
        pthread_cond_signal(&comida->philosophers[id]->left);

    if(comida->picks[comida->philosophers[id]->right] == 2)
      pthread_cond_signal(&comida->philosophers[id]->right);

    pthread_mutex_unlock(&comida->lock);
}

void think() {
    usleep(50*1000);
}

void eat(uint8_t id) {
    printf("%d empieza a comer", id);
    usleep(100*1000);
    printf("%d acaba de comer", id);
}
