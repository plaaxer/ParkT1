#include "shared.h"
#include <queue.h>
#include <semaphore.h>

// test_variable
sem_t clients_in_line;

pthread_mutex_t *clients_mutexes;
// Você pode declarar novas funções (ou variaveis compartilhadas) aqui

/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
Queue *gate_queue = NULL;
