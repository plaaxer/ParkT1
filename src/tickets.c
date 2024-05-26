/**
 * Esse arquivo tem como objetivo a implementação da bilheteria em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES FORNECIDAS.
*/
#include <stdio.h>
#include <stdlib.h>
#include <tickets.h>
#include <unistd.h>

#include <queue.h>
#include "shared.h"


// Thread que implementa uma bilheteria
void *sell(void *args){

    debug("[INFO] - Bilheteria Abriu!\n");

    // Murta
    // Enquanto fila nao-vazia
    while (!is_queue_empty(gate_queue)){
        // Atende um cliente
        int id = dequeue(gate_queue);
        debug("[INFO] - Cliente [%d] atendido.\n", id);
        // Libera o cliente
        // funcao para liberar o cliente AQUI
        // Dorme por um tempo
        sleep(rand() % 5);
    }

    pthread_exit(NULL);
}

// Essa função recebe como argumento informações sobre a bilheteria e deve iniciar os atendentes.
void open_tickets(tickets_args *args){
    // Murta
    // Fila da bilheteria ja criada em main (gate_queue)
    // Cria N threads (N funcionários da bilheteria)
    pthread_t *tickets = malloc(args->n * sizeof(pthread_t));
    for (int i = 0; i < args->n; i++){
        pthread_create(&tickets[i], NULL, sell, NULL);
    }
    // Espera todas as threads finalizarem
    for (int i = 0; i < args->n; i++){
        pthread_join(tickets[i], NULL);
    }
    // Libera a memória
    free(tickets);
}

// Essa função deve finalizar a bilheteria
void close_tickets(){
    pthread_exit(NULL);
}