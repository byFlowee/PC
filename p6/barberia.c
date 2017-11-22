#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX_CLIENTES 20
#define PLAZAS_SOFA 4
#define NUM_BARBEROS 3
#define NUM_CLIENTES 30

int currentClient = 0;  //
int sofaClient = 0;     // Variables de control que se acceden en exclusion mutua
int chairClient = 0;    //

struct tdata {
    int tid;
};

sem_t terminado;
sem_t dejar_silla;
sem_t cliente_listo;
sem_t pago;
sem_t recibo;
sem_t max_clientes;
sem_t sofa;
sem_t silla_barbero;
sem_t coord_barbero;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  //mutex para acceder a las variables de control

/*wait(pago)
  wait(coord)
  cobrar()
  signal(coord)
  signal(recibo)
*/

void *cajero(void *ptr) {
    int id = ((struct tdata *) ptr)->tid - NUM_BARBEROS;
    while(1) {
        sem_wait(&pago);  //Bloquea al cajero hasta que el cliente pague
        sem_wait(&coord_barbero); //Bloquea al barbero si hay 3 barberos ocupados (cortando el pelo o en caja)

        printf("\n...\n");
        printf("El barbero %d cobra\n", id);
        printf("...\n\n");
        usleep(500*1000);

        sem_post(&coord_barbero); //Libera para indicar que el barbero está libre
        sem_post(&recibo);  //Libera para indicar al cliente que ya tiene su recibo
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
    int id = ((struct tdata *) ptr)->tid + 1;
    while(1) {
        sem_wait(&cliente_listo); //Bloquea al barbero hasta que el cliente esté sentado en la silla
        sem_wait(&coord_barbero); //Bloquea al barbero si hay 3 barberos ocupados (cortando el pelo o en caja)

        printf("\n...\n");
        printf("El barbero %d corta el pelo\n", id);
        printf("...\n");
        usleep(500*1000);

        sem_post(&coord_barbero); //Libera para indicar que el barbero está libre
        sem_post(&terminado); //Libera para indicar al cliente que ha acabado
        sem_wait(&dejar_silla); //Bloquea al barbero hasta que el cliente se levante de la silla
        sem_post(&silla_barbero); //Libera la silla para que otros clientes puedan sentarse
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
  wait(recibo);chairClient
  signal(max_capacidad);
*/

void *client(void *ptr) {
    int id = ((struct tdata *) ptr)->tid + 1;
    usleep(100*1000);

    sem_wait(&max_clientes);  //Bloquea la entrada cuando no queda sitio en la tienda, inicializado a la capacidad maxima de clientes (20)

    usleep(100*1000);

    pthread_mutex_lock(&mutex);
    currentClient++;
    pthread_mutex_unlock(&mutex);

    printf("Cliente %d entra en la barberia, hay %d clientes en la tienda\n", id, currentClient);

    sem_wait(&sofa);  //Bloquea la entrada al sofa si no hay sitio, inicializado a la capacidad máxima del sofa (4)

    pthread_mutex_lock(&mutex);
    ++sofaClient;
    pthread_mutex_unlock(&mutex);

    printf("Cliente %d se sienta en el sofa, hay %d clientes sentados en el sofa\n", id, sofaClient);
    usleep(100*1000);

    sem_wait(&silla_barbero); //Bloquea la entrada a las sillas de los barberos si no hay ninguna libre, inicializado al número de sillas (3, una por barbero)

    pthread_mutex_lock(&mutex);
    --sofaClient;
    pthread_mutex_unlock(&mutex);

    printf("Cliente %d se levanta del sofa, hay %d clientes sentados en el sofa\n", id, sofaClient);

    sem_post(&sofa);  //El cliente se levanta del sofa, liberamos el semaforo para que otro cliente se siente

    pthread_mutex_lock(&mutex);
    ++chairClient;
    pthread_mutex_unlock(&mutex);

    printf("Cliente %d se sienta en la silla del barbero, hay %d sillas ocupadas\n", id, chairClient);
    usleep(100*1000);

    sem_post(&cliente_listo);   //El cliente se ha sentado en la silla del barbero, hacemos signal para desbloquear al barbero y que corte el pelo
    sem_wait(&terminado);   //Bloquea al cliente hasta el barbero haya terminado de cortar el pelo

    pthread_mutex_lock(&mutex);
    --chairClient;
    pthread_mutex_unlock(&mutex);

    printf("Cliente %d se levanta de la silla del barbero, hay %d sillas ocupadas\n", id, chairClient);
    usleep(100*1000);

    sem_post(&dejar_silla); //El cliente se ha levantado de la silla, liberamos el semaforo para que el barbero continue con otra tarea
    sem_post(&pago);  //Indica al cajero que va a pagar
    sem_wait(&recibo);  //Espera a que haga el recibo

    printf("Cliente %d sale de la barberia, quedan %d clientes\n", id, --currentClient);

    sem_post(&max_clientes);  //El cliente sale de la tienda, liberamos el semaforo para otros clientes puedan entrar
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
    sem_init(&terminado, 0, 0);
    sem_init(&dejar_silla, 0, 0);
    sem_init(&pago, 0, 0);
    sem_init(&recibo, 0, 0);
    sem_init(&coord_barbero, 0, NUM_BARBEROS);

    for(i=0; i<NUM_CLIENTES; i++){
        id2[i].tid = i;
        usleep((rand() % 20 + 1)*1000);   //Espera un numero aleatorio entre 1 y 20 milisegundos entre cada cliente
        rc = pthread_create(&th_clientes[i], NULL, client, (void *) &id2[i]);
    }

    for(i=0; i<NUM_BARBEROS; i++){  // Ejecutamos 2*NUM_BARBEROS Hilos para separar el codigo de barberos y cajeros,
                                    // controlamos mediante el semaforo coord_barbero que solo NUM_BARBEROS puedan realizar tareas simultaneamente
        id[i].tid = i;
        id[NUM_BARBEROS + i].tid = NUM_BARBEROS + i;
        pthread_create(&th_barberos[i], NULL, barbero, (void *) &id[i]);  //Hilos de barberos
        pthread_create(&th_barberos[NUM_BARBEROS + i], NULL, cajero, (void *) &id[NUM_BARBEROS + i]); //Hilos de cajeros
    }

    for(i=0; i<NUM_CLIENTES; i++){  //Esperamos a que todos los clientes acaben
        pthread_join(th_clientes[i], NULL);
    }

    printf("\nHa salido el ultimo cliente, se cierra la tienda\n");

    return 0;
}
