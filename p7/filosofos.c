#include "comida.h"

comida_t comida;

void *run(void *arg) {
    uint8_t id = *((uint8_t *)arg);
    int i;

    for (i = 0; i < EAT_COUNT; ++i) {
        think();
        pick(id, &comida);
        eat(id);
        release(id, &comida);
    }
}

int main() {
    pthread_t ph[PHILOSOPHERS];
    setup(&comida);

    uint8_t i;

    for (i = 0; i < PHILOSOPHERS; ++i) {
        pthread_create(&ph[i], NULL, run, (void *)&i);
    }

    for (i = 0; i < PHILOSOPHERS; ++i) {
        pthread_join(ph[i], NULL);
    }

    return 0;
}
