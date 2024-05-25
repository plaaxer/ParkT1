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

    pthread_exit(NULL);
}

// Essa função recebe como argumento informações sobre a bilheteria e deve iniciar os atendentes.
void open_tickets(tickets_args *args){
    // Murta
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
    // /Murta
}

// Essa função deve finalizar a bilheteria
void close_tickets(){
    // Como finalizar as threads acima sem alterar os parametros das funções? e sem variáveis globais?
    pthread_exit(NULL);
}