#ifndef _COMIDA_H_
#define _COMIDA_H_

#include <sys/types.h>
#include <pthread.h>
#include <stdint.h>

#define PHILOSOPHERS 5
#define EAT_COUNT 100

typedef struct {
    uint8_t id;
    uint8_t right;
    uint8_t left;
}philosopher_t;

typedef struct {
    uint8_t picks[PHILOSOPHERS];
    pthread_cond_t canEat[PHILOSOPHERS];
    pthread_mutex_t lock;
    philosopher_t philosophers[PHILOSOPHERS];
}comida_t;

#define PHILOSOPHER_INIT(id) {\
    (id),\
    (id) - 1 % PHILOSOPHERS,\
    (id) + 1 % PHILOSOPHERS\
}

void setup(comida_t *comida);

void pick(uint8_t id, comida_t *comida);
void release(uint8_t id, comida_t *comida);

void think();
void eat(uint8_t id);

#endif
