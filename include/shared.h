#ifndef __SHARED_H__
#define __SHARED_H__

#include <defs.h>

#include <semaphore.h>

extern pthread_mutex_t queue_mutex;
extern sem_t* client_semaphores;
extern int number_of_clients; // numero de clientes

extern sem_t clients_on_queue; // controlar a quantidade de clientes na fila

  
// Você pode declarar novas funções (ou variaveis compartilhadas) aqui



/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
extern Queue *gate_queue;
#endif
