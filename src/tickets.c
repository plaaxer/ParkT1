/**
 * Esse arquivo tem como objetivo a implementação da bilheteria em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES
 * FORNECIDAS.
 */
#include <stdio.h>
#include <stdlib.h>
#include <tickets.h>
#include <unistd.h>

#include "shared.h"
#include <queue.h>

int ended = 0;

void initialize_ticketing(){
  sem_init(&clients_in_line, 0 , 0);
}

// Thread que implementa uma bilheteria
void *sell(void *args) {
  ticket_t *ticket = (ticket_t*)args; 

  debug("[INFO] - Bilheteria [%d] Abriu!\n", ticket->id);

  // Murta
  // Enquanto fila nao-vazia
  
  while(!ended){
  // semaforo para esperar ter clientes na fila para prosseguir (evita busy waiting)
    int coisa;
    sem_getvalue(&clients_in_line, &coisa);
    debug("[SEMAPHOR] - bilheteria [%d] // valor do semaforo: [%d]\n", ticket->id, coisa);
    sem_wait(&clients_in_line);

    // verificacao de se tem clientes na fila (vai sempre ser verdadeiro menos quando close_tickets ja foi chamado)
    if (!is_queue_empty(gate_queue)){
      int id = dequeue(gate_queue);
      debug("[LOOP] - Bilheteria [%d] atendendo cliente [%d]\n", ticket->id, id);
      
      // libera o cliente para entrar no parque
      pthread_mutex_unlock(&clients_mutexes[id-1]);

    }
  }
  debug("[INFO] - Bilheteria [%d] Fechou!\n", ticket->id);
  pthread_exit(NULL);
}




// Essa função recebe como argumento informações sobre a bilheteria e deve
// iniciar os atendentes.
void open_tickets(tickets_args *args) {
  initialize_ticketing();

  // Murta
  // Fila da bilheteria ja criada em main (gate_queue)
  // Cria N threads (N funcionários da bilheteria)

  for (int i = 0; i < args->n; i++) {
    pthread_create(&args->tickets[i]->thread, NULL, sell, (void*) args->tickets[i]);
  }

  // Espera todas as threads finalizarem
  for (int i = 0; i < args->n; i++) {
    pthread_join(args->tickets[i]->thread, NULL);
  }
}




// Essa função deve finalizar a bilheteria
void close_tickets() {
    debug("[INFO] - Bilheteria Fechou!\n");
    sem_post(&clients_in_line); // apenas para liberar a bilheteria do trabalho para poder terminar
    ended = 1;
}
