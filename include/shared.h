#ifndef __SHARED_H__
#define __SHARED_H__

#include <defs.h>

#include <semaphore.h>

extern sem_t available_tickets;
extern sem_t available_clients;

void initialize_ticketing();
  
// Você pode declarar novas funções (ou variaveis compartilhadas) aqui



/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
extern Queue *gate_queue;
#endif
