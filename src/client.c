/**
 * Esse arquivo tem como objetivo a implementação do cliente em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES
 * FORNECIDAS
 */

#include <stdio.h>
#include <stdlib.h>

#include "client.h"
#include "defs.h"
#include "queue.h"
#include "shared.h"
#include <unistd.h>

pthread_t *clients_t;
pthread_mutex_t mutex_enqueue = PTHREAD_MUTEX_INITIALIZER;

// ========= Funções dadas =============

// Thread que implementa o fluxo do cliente no parque.
void *enjoy(void *arg) {

  // MURTA
  client_t *client =
      (client_t *)arg; // casting do argumento na struct do cliente

  queue_enter(client);
  sleep(1);
  debug("[ENTER] Turista [%d] entrou no parque com [%d] moedas.\n", client->id,
        client->coins);

  sleep(1);

  debug("[ENTER] Turista [%d] ta brincando muito feliz!\n", client->id);

  sleep(1);

  debug("[EXIT] - O turista saiu do parque.\n");
  pthread_exit(NULL);
}

// Funcao onde o cliente compra as moedas para usar os brinquedos
void buy_coins(client_t *self) {
  // Murta
  self->coins = rand() % (MAX_COINS - 1) + MIN_COINS;

  debug("[CASH] - Turista [%d] comprou [%d] moedas.\n", self->id, self->coins);
}

// Função onde o cliente espera a liberacao da bilheteria para adentrar ao
// parque.
void wait_ticket(client_t *self) {

  // cliente vai ser travado ate que a bilheteria o libere
  pthread_mutex_lock(&clients_mutexes[self->id - 1]);
  pthread_mutex_lock(&clients_mutexes[self->id - 1]);
}

// Funcao onde o cliente entra na fila da bilheteria
void queue_enter(client_t *self) {

  // Cliente entra na fila da bilheteria --> mutex para protecao
  pthread_mutex_lock(&mutex_enqueue);
  enqueue(gate_queue, self->id);
  sem_post(&clientes_na_fila);
  pthread_mutex_unlock(&mutex_enqueue);

  // Cliente vai esperar ate ser atendido.
  wait_ticket(self);

  buy_coins(self);
}

// Essa função recebe como argumento informações sobre o cliente e deve iniciar
// os clientes.
void open_gate(client_args *args) {

  // Aloca espaco para N threads (N clientes)
  clients_t = malloc(args->n * sizeof(pthread_t));
  number_of_clients = args->n;

  // Aloca e inicializa os mutexes
  clients_mutexes = malloc(number_of_clients * sizeof(pthread_mutex_t));
  for (int i = 0; i < number_of_clients; i++)
    pthread_mutex_init(&clients_mutexes[i], NULL);


  debug("[INIT] - Inicializando lista de mutex\n");

  // Cria N threads (N clientes)
  for (int i = 0; i < number_of_clients; i++) {
    pthread_create(&clients_t[i], NULL, enjoy, args->clients[i]);
  }
}

// Essa função deve finalizar os clientes
void close_gate() {
  
  // Espera todas as threads finalizarem
  for (int i = 0; i < number_of_clients; i++) 
    pthread_join(clients_t[i], NULL);

  // Libera a memória
  free(clients_t);
  free(clients_mutexes);
}
