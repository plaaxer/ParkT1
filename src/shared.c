#include "shared.h"
#include <queue.h>
#include <semaphore.h>

pthread_mutex_t *clients_mutexes;
int number_of_clients = 0;
sem_t clientes_na_fila;

/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
Queue *gate_queue = NULL;
