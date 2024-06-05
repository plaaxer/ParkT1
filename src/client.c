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

// ========= Funções dadas =============

void playing(toy_t *toy, int client_id) {
 
  // ENTRA FILA DO BRINQUEDO
  // Mutex para bloquear a condicao de corrida da inicializacao do brinquedo alem de esperar um cliente. Explicacao no toy.c
  debug("[MUTEX] - Cliente [%d] travado no mutex_start do brinquedo [%d].\n", client_id, toy->id);
  //pthread_mutex_lock(&toy->mutex_start);
  //TEMP
  int sem_capacity_value;
  sem_getvalue(&toy->sem_capacity, &sem_capacity_value);
  printf("The value of sem_capacity is: %d\n", sem_capacity_value);

  debug("[SEM] - Cliente [%d] travado no sem_capacity do brinquedo [%d].\n", client_id, toy->id);
  sem_wait(&toy->sem_capacity);
  //pthread_mutex_unlock(&toy->mutex_start);

  // FALA QUE TEM ALGUEM
  debug("[MUTEX] - Cliente [%d] travado no mutex_ready do brinquedo [%d].\n", client_id, toy->id);
// Mutex para bloquear a condicao de corrida da inicializacao do brinquedo alem de esperar um cliente. Explicacao no toy.c
  pthread_mutex_lock(&toy->mutex_ready);
  // A variavel ready eh utilizada para evitar Spurious Wakeups, alem para outro caso explicado em toy.c.
  toy->ready = 1;
  // Sinaliza ao brinquedo que ha um cliente dentro dele e que ele poderia comecar a rodar.
  pthread_cond_broadcast(&toy->ready_to_start);
  pthread_mutex_unlock(&toy->mutex_ready);

    // ESPERA PODER COMECAR A SE DIVERTIR
  debug("[WAITING] - Cliente [%d] esperando o brinquedo [%d] iniciar.\n", client_id, toy->id);
  // Mutex condicional para que o cliente espere o brinquedo iniciar. While (!toy->running) para evitar Spurious Wakeups.
  pthread_mutex_lock(&toy->mutex_cond);

  toy->ready_clients++;
  pthread_cond_signal(&toy->all_clients_ready);
  while (!toy->running){
    debug("[WAITING] - Cliente [%d] Esperando dentro do cond [%d]. Valor de ready_clients: [%d] \n", client_id, toy->id, toy->ready_clients);
    pthread_cond_wait(&toy->cond, &toy->mutex_cond);
    debug("[TEMP] - Cliente [%d] foi liberado temporariamente no toy [%d]\n", client_id, toy->id);
  }
  debug("[COND] - Cliente [%d] liberado para brincar no brinquedo [%d].\n", client_id, toy->id);

  pthread_mutex_unlock(&toy->mutex_cond);

  // BRINQUEDO FUNCIONANDO

  debug("[PLAY] - Cliente [%d] brincando no brinquedo [%d].\n", toy->id, client_id);
  sleep(0); // duracao da brinquadeira

  pthread_mutex_lock(&toy->mutex_cond);
  toy->done_clients++;
  pthread_cond_signal(&toy->ready_to_end);
  pthread_mutex_unlock(&toy->mutex_cond);
} 

// Thread que implementa o fluxo do cliente no parque.
void *enjoy(void *arg) {

  client_t *client =
      (client_t *)arg; // casting do argumento na struct do cliente

  queue_enter(client);
  sleep(0);
  debug("[ENTER] Turista [%d] entrou no parque com [%d] moedas.\n", client->id,
        client->coins);

  sleep(0);

  while (client->coins > 0) {
    int toy_id = rand() % client->number_toys;
    playing(client->toys[toy_id], client->id);
    debug("Moedas restantes: %d\n", client->coins);
    client->coins--;
  }


  sleep(0);

  debug("[EXIT] - O cliente [%d] saiu do parque.\n", client->id);
  pthread_exit(NULL);
}

// Funcao onde o cliente compra as moedas para usar os brinquedos
void buy_coins(client_t *self) {

  self->coins = rand() % (MAX_COINS - 1) + MIN_COINS;

}

// Função onde o cliente espera a liberacao da bilheteria para adentrar ao
// parque.
void wait_ticket(client_t *self) {
  // cliente vai ser travado ate que a bilheteria o libere //ALTERAR PARA SEMAFORO BINARIO DEPOIS
  sem_wait(&(client_semaphores[self->id - 1]));
}

// Funcao onde o cliente entra na fila da bilheteria
void queue_enter(client_t *self) {

  // Cliente entra na fila da bilheteria --> mutex para protecao
  pthread_mutex_lock(&queue_mutex);
  enqueue(gate_queue, self->id);
  sem_post(&clients_on_queue);
  pthread_mutex_unlock(&queue_mutex);

  // Cliente vai esperar ate ser atendido.
  wait_ticket(self);

  buy_coins(self);

  debug("[CASH] - Turista [%d] comprou [%d] moedas.\n", self->id, self->coins);

}

// Essa função recebe como argumento informações sobre o cliente e deve iniciar
// os clientes.
void open_gate(client_args *args) {

  // Aloca espaco para N threads (N clientes)
  clients_t = malloc(args->n * sizeof(pthread_t));
  number_of_clients = args->n;

  // Aloca e inicializa os mutexes
  client_semaphores = malloc(number_of_clients * sizeof(sem_t));
  for (int i = 0; i < number_of_clients; i++)
    sem_init(&client_semaphores[i], 0, 0);


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
  free(client_semaphores);
}
