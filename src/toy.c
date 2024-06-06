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
#include <signal.h>

#include "shared.h"
#include "toy.h"

toy_args* toys_info;

int ON = 1; // Variavel global para ligar/desligar os brinquedos


// Thread que o brinquedo vai usar durante toda a simulacao do sistema
void *turn_on(void *args) {
  // Casting do argumento na struct do brinquedo
  toy_t *toy = (toy_t *)args; 

  // Variavel para armazenar a quantidade de clientes que irao brincar
  int current_clients = 0;

  // Variavel para armazenar a quantidade de clientes que faltam para atingir a capacidade maxima do brinquedo
  int missing;
  
  debug("[ON] - O brinquedo  [%d] foi ligado.\n",
        toy->id);

  while (ON){
    
    // ESPERANDO CLIENTES
    sleep(1);

    // ANTES DE COMECAR A RODAR

    // Aqui ocorre a verificacao de se ha clientes no brinquedo. Se nao houver, o brinquedo nao pode rodar. Obviamente, nao ha busy waiting
    // pois o brinquedo espera por um sinal para comecar a rodar --> o toy->ready eh para evitar Spurious Wakeups.
    pthread_mutex_lock(&toy->mutex_ready);
    while (!toy->ready) {
      pthread_cond_wait(&toy->ready_to_start, &toy->mutex_ready);
    }
    pthread_mutex_unlock(&toy->mutex_ready);
    
    // Aqui, pegamos o valor de quantos clientes faltam para a capacidade maxima (missing) e preenchemos o semaforo para que nenhum cliente
    // entre enquanto o brinquedo roda. Tambem pegamos o valor de current_clients que eh quantos clientes irao brincar nessa rodada.
    
    sem_getvalue(&toy->sem_capacity, &missing);
    for (int i = 0; i < missing; i++) {
      sem_wait(&toy->sem_capacity);
    }
    current_clients = toy->capacity - missing;

    // COMECA A RODAR

    // Primeiro bloqueamos o mutex_condicional para poder atualizarmos a variavel de condicao "cond" e liberar os clientes que estavam esperando
    // o brinquedo rodar. Alem disso setamos a variavel auxiliar running para 1, indicando que o brinquedo esta rodando. A running eh
    // utilizada para evitar Spurious Wakeups - e tambem eh util em alguns outros casos.

    pthread_mutex_lock(&toy->mutex_cond);

    // Aqui aproveitamos o mesmo mutex para utilizar outra variavel condicional chamada "all_clients_ready", que indica quando todos os clientes
    // que irao brincar nessa rodada estao prontos dentro do brinquedo. Estar "pronto" significa estar apenas esperando o sinal - ou melhor,
    // broadcast - para poder comecar a brincar. Toda vez que um cliente fica "pronto" ele testa a condicao while(ready_clients<curren_clients),
    // e, caso falsa, isso indica que ele poderia comecar a rodar.
    
    while (toy->ready_clients < current_clients)
      pthread_cond_wait(&toy->all_clients_ready, &toy->mutex_cond);
    
    toy->ready_clients = 0;
    toy->running = 1;
    pthread_cond_broadcast(&toy->cond);
    pthread_mutex_unlock(&toy->mutex_cond);

    // Ou seja -> o brinquedo espera todo mundo ficar pronto, que eh quando &all_clients_ready o libera. Depois, ele libera todo mundo para brincar,
    // atraves do broadcast em &cond.

    // RODANDO

    debug("[TOY] - Brinquedo [%d] rodando, foram ocupados [%d/%d] lugares.\n", toy->id, missing, toy->capacity);
    sleep(0);
    
    // ESPERA PARA N TERMINAR COM ALGUEM PRESO
    // Aqui ocorre algo parecido com o inicio, so que dessa vez, ao inves de esperar todos ficarem prontos, agora eh esperar nao ter ninguem mais brincando.
    // Isso significa que o brinquedo pode parar de rodar e ir para a proxima etapa. Dessa vez, &ready_to_end e int done_clients sao utilizados como controles.
    
    pthread_mutex_lock(&toy->mutex_cond);
    while (toy->done_clients < current_clients){
      pthread_cond_wait(&toy->ready_to_end, &toy->mutex_cond);
    }
    toy->done_clients = 0;
    pthread_mutex_unlock(&toy->mutex_cond);

    //TERMINANDO E ESVAZIANDO O BRINQUEDO
    // Aqui setamos a variavel running para 0 (brinquedo parou de rodar) e liberamos os semaforos para que os clientes possam entrar (semaforo valera a capacidade do brinquedo).
    // Alem disso, setamos ready a 0 novamente para indicar que o brinquedo tem que esperar alguem entrar para iniciar novamente.
    toy->running = 0;
    toy->ready = 0;
    for (int i = 0; i < toy->capacity; i++) {
      sem_post(&toy->sem_capacity);
    }
  }


  debug("[OFF] - O brinquedo [%d] foi desligado.\n", toy->id);

  pthread_exit(NULL);
}


// Essa função recebe como argumento informações e deve iniciar os brinquedos.
void open_toys(toy_args *args) {
  // guardamos as informações do brinquedos 
  toys_info = args;

  // Cria N threads (N brinquedos)
  for (int i = 0; i < toys_info->n; i++){
    toy_t* toy = toys_info->toys[i];

    pthread_mutex_init(&toy->mutex_start, NULL);    // inicializa o mutex que controla o bloqueio quando o brinquedo roda
    pthread_mutex_init(&toy->mutex_cond, NULL);     // inicializa o mutex auxiliar para a variavel de condicao para evitar condicao de corrida
    pthread_mutex_init(&toy->mutex_ready, NULL);    // inicializa o mutex auxiliar para a variavel de condicao de start
    sem_init(&toy->sem_capacity, 0, toy->capacity); // inicializa o semaforo que controla a capacidade do brinquedo
    
    pthread_create(&(toy->thread), NULL, turn_on, toy);
  }
}


// Função para desligar os brinquedos
void close_toys() {
  // sinalização para as threads pararem seu loop de execução.
  ON = 0;

 // força o término da thread ao final da execução do código.
  for (int i = 0; i < toys_info->n; i++) 
    pthread_cancel(toys_info->toys[i]->thread);

  debug("[CLOSE] Os brinquedos foram finalizados!\n");
}

// Desligando os brinquedos
