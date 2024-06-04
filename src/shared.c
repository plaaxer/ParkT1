#include "shared.h"
#include <queue.h>
#include <semaphore.h>

pthread_mutex_t *clients_mutexes;
int number_of_clients = 0;
// Você pode declarar novas funções (ou variaveis compartilhadas) aqui

/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
Queue *gate_queue = NULL;
