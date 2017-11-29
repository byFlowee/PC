#ifndef _COMIDA_H_
#define _COMIDA_H_

#include <sys/types.h>
#include <pthread>

#define PHILOSOPHERS 5
#define EAT_COUNT 100

typedef struct comida {
    uint8_t picks[];
    uint8_t canEat[];
    uint8_t count = 0;
    philosopher_t philosofers[PHILOSOPHERS];
}comida_t;

typedef struct philosopher {
    uint8_t id;
    uint8_t right;
    uint8_t left;
}philosopher_t;

#define PHILOSOPHER_INIT(id) {\
    (id),\
    (id) - 1 % PHILOSOPHERS,\
    (id) + 1 % PHILOSOPHERS
}

void pick();
void release();

void think();
void eat();

void update();
