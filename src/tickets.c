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

int n_clients;
tickets_args* tickets_info;

// Thread que implementa uma bilheteria
void *sell(void *args) {

  // casting de void para a estrutura da bilheteria
  ticket_t *ticket = (ticket_t*)args; 

  debug("[INFO] - Bilheteria [%d] Abriu!\n", ticket->id);

  while(n_clients > 0){
    sem_wait(&clients_on_queue);

    // verificacao de se tem clientes na fila (podem ter mais clientes mas n estao na fila ainda)
    if (!is_queue_empty(gate_queue)){

      pthread_mutex_lock(&queue_mutex);
      // pega o cliente da fila
      int id = dequeue(gate_queue);
      
      // decrementa o numero de clientes
      n_clients--;
      pthread_mutex_unlock(&queue_mutex);

      // debugging
      debug("[INFO] - Bilheteria [%d] Atendendo cliente [%d]\n", ticket->id, id);

      // libera o cliente para entrar no parque
      sem_post(&client_semaphores[id-1]);

    }
  }
  // Quando uma bilheteria chega aqui, significa que todos os clientes já foram atendidos
  // Portanto o sem_post é chamado para liberar bilheterias/threads que podem estar "presas"

  sem_post(&clients_on_queue);
  debug("[INFO] - Bilheteria [%d] Fechou!\n", ticket->id);
  pthread_exit(NULL);
}




// Essa função recebe como argumento informações sobre a bilheteria e deve
// iniciar os atendentes.
void open_tickets(tickets_args *args) {

  // Fila da bilheteria ja criada em main (gate_queue)
  tickets_info = args;

  // passando variavel global para alteracao local
  n_clients = number_of_clients;

  // Cria N threads (N funcionários da bilheteria)

  for (int i = 0; i < args->n; i++) {
    pthread_create(&args->tickets[i]->thread, NULL, sell, (void*) args->tickets[i]);
  }

}

// Essa função deve finalizar a bilheteria
void close_tickets() {

  // Espera todas as threads finalizarem
  for (int i = 0; i < tickets_info->n; i++) {
    pthread_join(tickets_info->tickets[i]->thread, NULL);
  }

    debug("[INFO] - Bilheteria Fechou!\n");

}
