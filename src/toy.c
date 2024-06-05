/**
 * Esse arquivo tem como objetivo a implementação de um brinquedo em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES
 * FORNECIDAS
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "shared.h"
#include "toy.h"

toy_args* toys_info;

int ON = 1; // Variavel global para ligar/desligar os brinquedos


// Thread que o brinquedo vai usar durante toda a simulacao do sistema
void *turn_on(void *args) {

  toy_t *toy = (toy_t *)args; // Casting do argumento na struct do brinquedo

  int current_clients = 0; // Variavel para armazenar a quantidade de clientes que irao brincar

  int missing; // Variavel para armazenar a quantidade de clientes que faltam para atingir a capacidade maxima do brinquedo

  pthread_mutex_init(&toy->mutex_start, NULL); // inicializa o mutex que controla o bloqueio quando o brinquedo roda
  pthread_mutex_init(&toy->mutex_cond, NULL); // inicializa o mutex auxiliar para a variavel de condicao para evitar condicao de corrida
  pthread_mutex_init(&toy->mutex_ready, NULL); // inicializa o mutex auxiliar para a variavel de condicao de start
  sem_init(&toy->sem_capacity, 0, toy->capacity); // inicializa o semaforo que controla a capacidade do brinquedo

  debug("[ON] - O brinquedo  [%d] foi ligado.\n",
        toy->id);

  while (ON){
    
    // ESPERANDO CLIENTES
    sleep(0);

    // ANTES DE COMECAR A RODAR

    // Aqui ocorre a verificacao de se ha clientes no brinquedo. Se nao houver, o brinquedo nao pode rodar. Obviamente, nao ha busy waiting
    // pois o brinquedo espera por um sinal para comecar a rodar --> o toy->ready eh para evitar Spurious Wakeups.
    pthread_mutex_lock(&toy->mutex_ready);
    while (!toy->ready) {
      pthread_cond_wait(&toy->ready_to_start, &toy->mutex_ready);
    }
    pthread_mutex_unlock(&toy->mutex_ready);

    // Aqui, pegamos o valor de quantos clientes faltam para a capacidade maxima e preenchemos o semaforo para que nenhum cliente
    // entre enquanto o brinquedo roda. O mutex eh para que nenhum cliente entre, o que possivelmente tornaria o valor de getValue
    // incorreto.
    pthread_mutex_lock(&toy->mutex_start);
    sem_getvalue(&toy->sem_capacity, &missing);
    for (int i = 0; i < missing; i++) {
      sem_wait(&toy->sem_capacity);
    }
    debug("[TOY] - Brinquedo [%d] fechou o brinquedo. [%d] lugares restantes.\n", toy->id, missing);
    current_clients = toy->capacity - missing;
    pthread_mutex_unlock(&toy->mutex_start);

    // COMECA A RODAR
    // Aqui bloqueamos o mutex_condicional para poder atualizarmos a variavel de condicao e liberar os clientes que estavam esperando
    // o brinquedo rodar. Alem disso setamos a variavel auxiliar running para 1, indicando que o brinquedo esta rodando. A running eh
    // utilizada para evitar Spurious Wakeups.

    pthread_mutex_lock(&toy->mutex_cond);
    debug("[TOY] - Brinquedo [%d] testando. Current clients: [%d]. Valor de running: [%d]. Valor de ready_clients: [%d]\n", toy->id, current_clients, toy->running, toy->ready_clients);
    while (toy->ready_clients < current_clients){
      pthread_cond_wait(&toy->all_clients_ready, &toy->mutex_cond);
    }
    debug("[TOY] - Brinquedo [%d} liberou todos os clientes para brincar.\n", toy->id);
    toy->ready_clients = 0;
    toy->running = 1;
    pthread_cond_broadcast(&toy->cond);
    pthread_mutex_unlock(&toy->mutex_cond);

    // RODANDO
    debug("[TOY] - Brinquedo [%d] rodando --> [%d] lugares restantes --> [%d] capacidade maxima\n", toy->id, missing, toy->capacity);
    sleep(0);

    // ESPERA PARA N TERMINAR COM ALGUEM PRESO
    pthread_mutex_lock(&toy->mutex_cond);
    while (toy->done_clients < current_clients){
      pthread_cond_wait(&toy->ready_to_end, &toy->mutex_cond);
    }
    toy->done_clients = 0;
    pthread_mutex_unlock(&toy->mutex_cond);

    // Aqui setamos a variavel running para 0 (brinquedo parou de rodar). Bloqueamos o mutex_start para que nenhum cliente entre (pela
    // mesma razao do inicio) e liberamos os semaforos para que os clientes possam entrar (semaforo valera a capacidade do brinquedo).
    toy->running = 0;
    toy->ready = 0;
    for (int i = 0; i < toy->capacity; i++) {
      sem_post(&toy->sem_capacity);
    }
  }


  debug("[OFF] - O brinquedo [%d] foi desligado.\n",
        toy->id);

  pthread_exit(NULL);
}





// Essa função recebe como argumento informações e deve iniciar os brinquedos.
void open_toys(toy_args *args) {
  toys_info = args;

  // Cria N threads (N brinquedos)
  for (int i = 0; i < args->n; i++) 
    pthread_create(&(args->toys[i]->thread), NULL, turn_on, args->toys[i]);

}


// Função para desligar os brinquedos
void close_toys() {
  ON = 0;
}

// Desligando os brinquedos
