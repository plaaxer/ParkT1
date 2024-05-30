/**
 * Esse arquivo tem como objetivo a implementação do cliente em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES FORNECIDAS
*/

#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "client.h"
#include "queue.h"
#include "shared.h"

pthread_t *clients;
int n_clients;

// Thread que implementa o fluxo do cliente no parque.
void *enjoy(void *arg){

    // MURTA
    client_t *client = (client_t *)arg; // casting do argumento na struct do cliente
    debug("[ENTER] Turista [%d] entrou na fila com [%d] moedas.\n", client->id, client->coins);
    queue_enter(client);
    debug("[ENTER] - Turista [%d] entrou no parque.\n", client->id);
    printf("thread [%d] finalizada\n", client->id);

    // /MURTA
    while (1) {
        // Perform client activities here
    }
    debug("[EXIT] - O turista saiu do parque.\n");
    pthread_exit(NULL);
}

// Criador de clientes
void create_clients(client_args *args){
    // Cria N threads (N clientes)
    n_clients = args->n;
    pthread_t *clients = malloc(args->n * sizeof(pthread_t));
    for (int i = 0; i < args->n; i++){
        pthread_create(&clients[i], NULL, enjoy, args->clients[i]);
    }
    // Espera todas as threads finalizarem
    for (int i = 0; i < args->n; i++){
        pthread_join(clients[i], NULL);
    }
    // Libera a memória
    free(clients);
}

// Funcao onde o cliente compra as moedas para usar os brinquedos
void buy_coins(client_t *self){
    // Murta
    self->coins = rand() % (MAX_COINS - 1) + MIN_COINS;
    debug("[CASH] - Turista [%d] comprou [%d] moedas.\n", self->id, self->coins);
}

// Função onde o cliente espera a liberacao da bilheteria para adentrar ao parque.
void wait_ticket(client_t *self){
    // Murta
    debug("[WAITING] - Turista [%d] esperando na fila do portao principal\n", self->id);
    while (0); // AQUI ELE IRA ESPERAR ATE A BILHETERIA O LIBERAR / MAS COMO?
}


// Funcao onde o cliente entra na fila da bilheteria
void queue_enter(client_t *self){
    // Murta
    // Cliente entra na fila da bilheteria
    enqueue(gate_queue, self->id);
    wait_ticket(self);
    buy_coins(self);
    close_gate();
}

// Essa função recebe como argumento informações sobre o cliente e deve iniciar os clientes.
void open_gate(client_args *args){
    // MURTA
    create_clients(args);
    debug("[EXIT PARQUE DEPOIS DE DISPATCHER]\n")
    // /MURTA
}

// Essa função deve finalizar os clientes
void close_gate(){
    //murta
    debug("CLOSE_GATE FOI CHAMADO\n");
    for (int i = 0; i < n_clients; i++){
        pthread_cancel(clients[i]);
    }
}
