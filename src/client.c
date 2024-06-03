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
int n_clients; // n clients deve ser publico para que close_gates o acesse
pthread_mutex_t mutexteste;
int test_variable = 0;

void initializeClientMutexes() {
  // Murta
  // Inicializa os mutexes dos clientes
  clients_mutexes = malloc(n_clients * sizeof(pthread_mutex_t));
  for (int i = 0; i < n_clients; i++) {
    pthread_mutex_init(&clients_mutexes[i], NULL);
  }

}

// Criador de clientes
void create_clients(client_args *args) {

    // Cria N threads (N clientes)
    clients_t = malloc(args->n * sizeof(pthread_t));
    n_clients = args->n;

    initializeClientMutexes(); // cria uma lista de mutex para cada cliente
    debug("[INIT] - Inicializando lista de mutex\n");
    for (int i = 0; i < n_clients; i++){
        pthread_create(&clients_t[i], NULL, enjoy, args->clients[i]);
    }
}

// ========= Funções dadas ============= 



// Thread que implementa o fluxo do cliente no parque.
void *enjoy(void *arg) {

  // MURTA
  client_t *client = (client_t *)arg; // casting do argumento na struct do cliente
 
  queue_enter(client);
  sleep(1);
  debug("[ENTER] Turista [%d] entrou no parque com [%d] moedas.\n",
        client->id,
        client->coins);
  
  sleep(1);

  debug("[ENTER] Turista [%d] ta brincando muito feliz!\n",
      client->id);

  sleep(1);

  debug("[EXIT] - O turista saiu do parque.\n");
  pthread_exit(NULL);
}

// Funcao onde o cliente compra as moedas para usar os brinquedos
void buy_coins(client_t *self) {
  // Murta
  self->coins = rand() % (MAX_COINS - 1) + MIN_COINS;

  //debug("[CASH] - Turista [%d] comprou [%d] moedas.\n", 
          //self->id, 
          //self->coins);
}

// Função onde o cliente espera a liberacao da bilheteria para adentrar ao
// parque.
void wait_ticket(client_t *self) {
  int coisa;
  pthread_mutex_lock(&mutexteste);

  // Murta
  debug("[WAITING] - Turista [%d] esperando na fila do portao principal\n",
        self->id);

  // cliente indica que tem alguem na fila
  sem_getvalue(&clients_in_line, &coisa);
  debug("[SEMAPHORE] - cliente [%d] value BEFORE post: [%d]\n", self->id, coisa);

  sem_post(&clients_in_line);

  test_variable++;
  debug("[test_variable] - [%d]\n", test_variable);

  sem_getvalue(&clients_in_line, &coisa);
  debug("[SEMAPHORE] - cliente [%d] value AFTER post: [%d]\n", self->id, coisa);

  pthread_mutex_unlock(&mutexteste);

  // cliente vai ser travado ate que a bilheteria o libere
  pthread_mutex_lock(&clients_mutexes[self->id - 1]);
  pthread_mutex_lock(&clients_mutexes[self->id - 1]);
}

// Funcao onde o cliente entra na fila da bilheteria
void queue_enter(client_t *self) {

  // Murta
  // Cliente entra na fila da bilheteria
  enqueue(gate_queue, self->id);

  // Cliente vai esperar ate ser atendido.
  wait_ticket(self);

  buy_coins(self);

}

// Essa função recebe como argumento informações sobre o cliente e deve iniciar
// os clientes.
void open_gate(client_args *args) {
  // MURTA
  pthread_mutex_init(&mutexteste, NULL);
  create_clients(args);
  // /MURTA join ou enjoy
}



// Essa função deve finalizar os clientes
void close_gate() {
  
  debug("teste1\n")
  // Espera todas as threads finalizarem
  for (int i = 0; i < n_clients; i++){
      pthread_join(clients_t[i], NULL);
  }
  debug("teste2\n")
  // Libera a memória
    free(clients_t);

 
}
