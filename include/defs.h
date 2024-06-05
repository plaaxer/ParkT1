#ifndef __DEFS_H__
#define __DEFS_H__

/* Essa biblioteca implementa definicoes que sao usadas pelo programa. */
/* ATENÇÃO: NÃO APAGUE OU EDITE O NOMES FORNECIDOS */

#include <pthread.h>
#include <semaphore.h>

#define TRUE 1              // Em C nao temos True ou False (como no Python). Usamos macro TRUE para True (como no Python). 
#define FALSE 0             // e FALSE para False (como no Python).


#define MAX_CAPACITY_TOY    10  // Capacidade maxima dos brinquedos.
#define MIN_CAPACITY_TOY    5   // Capacidade minima dos brinquedos.
#define MAX_COINS           5  // Maximo de moedas que um cliente pode comprar
#define MIN_COINS           1   // Murta

#define DEBUG               1   //  Alterne (0 or 1) essa macro se voce espera desabilitar todas as mensagens de debug.


/* Adicione as estruturas de sincronização que achar necessárias */
typedef struct ticket{
  pthread_t thread;         // A thread do funcionário que fica na bilheteria.
  int id;                   // O id do funcionario que fica na bilheteria.
} ticket_t;

/* Adicione as estruturas de sincronização que achar necessárias */
typedef struct toy{
  int id;                   // O id de um brinquedo.
  int capacity;             // A capacidade total de um brinquedo.
  sem_t sem_capacity;       // Semáforo para controlar a capacidade do brinquedo (clientes dentro).
  pthread_mutex_t mutex_start; // Mutex para controlar a inicialização do brinquedo -> impede que novos clientes quando ele esta iniciando.
  pthread_mutex_t mutex_cond; // Mutex auxiliar para a variavel de condição que sera usada para o cliente esperar o brinquedo iniciar.
  pthread_mutex_t mutex_ready; // Mutex auxiliar para a variavel de condição que sera usada para o brinquedo esperar os clientes entrarem.
  pthread_cond_t cond;      // Variavel de condição para o cliente esperar o brinquedo iniciar.
  int running;              // Variavel que indica se o brinquedo esta rodando ou nao.
  pthread_cond_t ready_to_start;    // Variavel que indica se ha clientes no brinquedo para que ele possa iniciar.
  int ready;                // Variavel que indica se o brinquedo esta pronto para iniciar. Como no outro condicional, ha necessidade de duas variaveis para evitar Spurious Wakeups.
  pthread_t thread;         // A thread de um brinquedo.
} toy_t;

/* Adicione as estruturas de sincronização que achar necessárias */
typedef struct client{
  int id;                   // O id do cliente.
  int coins;                // Quantidade de moedas do cliente.
  int number_toys;          // Numero de brinquedos disponiveis.
  toy_t **toys;             // (Copy) Array de brinquedos.
} client_t;

/* Adicione as estruturas de sincronização que achar necessárias */
typedef struct{
  ticket_t **tickets;        // Array de funcionarios.                       
  int n;                    // Quantidade de funcionários da bilheteria.
} tickets_args;

/* Adicione as estruturas de sincronização que achar necessárias */
typedef struct{
  toy_t **toys;             // Array de brinquedos.
  int n;                    // Quantidade de brinquedos.
} toy_args;

/* Adicione as estruturas de sincronização que achar necessárias */
typedef struct{
  client_t **clients;       // Array de clientes.
  int n;                    // Quantidade de clientes.
} client_args;


/**************************************************
 *                    ATENÇÃO                     *
 *   NÃO EDITAR ESSE ARQUIVO A PARTIR DESTE PONTO *
 *                    ATENÇÃO                     *
 *************************************************/

// Estrutura da fila
typedef struct node {
  int data;                 // Dado da fila.
  struct node *next;        // Objeto do proximo item da fila.
} Node;

// Fila
typedef struct queue {
  Node *front;              // Primeiro elemento da fila.
  Node *rear;               // Ultimo elemento da fila.
} Queue;

#if DEBUG
#define debug(...) printf(__VA_ARGS__);
#else
#define debug(...);
#endif


#endif
