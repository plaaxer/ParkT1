#ifndef __SHARED_H__
#define __SHARED_H__

#include <defs.h>

#include <semaphore.h>

extern pthread_mutex_t *clients_mutexes; // mutexes dos clientes
extern sem_t clients_in_line; // n de clientes na fila da bilheteria

void initialize_ticketing();
  
// Você pode declarar novas funções (ou variaveis compartilhadas) aqui



/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
extern Queue *gate_queue;
#endif
