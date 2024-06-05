#include "shared.h"
#include <queue.h>
#include <semaphore.h>

sem_t* client_semaphores;
pthread_mutex_t queue_mutex;

int number_of_clients = 0;
sem_t clients_on_queue;

/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
Queue *gate_queue = NULL;
