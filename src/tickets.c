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

// Thread que implementa uma bilheteria
void *sell(void *args) {

  // casting de void para a estrutura da bilheteria
  ticket_t *ticket = (ticket_t*)args; 

  debug("[INFO] - Bilheteria [%d] Abriu!\n", ticket->id);

  // passando variavel global para alteracao local
  int n_clients = number_of_clients;

  while(n_clients > 0){

    // verificacao de se tem clientes na fila (podem ter mais clientes mas n estao na fila ainda)
    if (!is_queue_empty(gate_queue)){

      // pega o cliente da fila
      int id = dequeue(gate_queue);
      
      // debugging
      debug("[INFO] - Bilheteria [%d] Atendendo cliente [%d]\n", ticket->id, id);

      // libera o cliente para entrar no parque
      pthread_mutex_unlock(&clients_mutexes[id-1]);

      // decrementa o numero de clientes
      n_clients--;

    }
  }
  
  debug("[INFO] - Bilheteria [%d] Fechou!\n", ticket->id);
  pthread_exit(NULL);
}




// Essa função recebe como argumento informações sobre a bilheteria e deve
// iniciar os atendentes.
void open_tickets(tickets_args *args) {

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
}
