#include "shared.h"
#include <queue.h>
#include <semaphore.h>

// oi
sem_t available_clients;
sem_t available_tickets;

void initialize_ticketing(){
  sem_init(&available_clients, 0 , 0);
  sem_init(&available_tickets, 0 , 0);

}

// Você pode declarar novas funções (ou variaveis compartilhadas) aqui

/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
Queue *gate_queue = NULL;
