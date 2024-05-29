/**
 * Esse arquivo tem como objetivo a implementação de um brinquedo em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES FORNECIDAS
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "toy.h"
#include "shared.h"


// Thread que o brinquedo vai usar durante toda a simulacao do sistema
void *turn_on(void *args){

    // Murta
    toy_t *toy = (toy_t *)args; // Casting do argumento na struct do brinquedo

    debug("[ON] - O brinquedo  [%d] foi ligado.\n", toy->id); // Altere para o id do brinquedo FEITO

    // /Murta
    debug("[OFF] - O brinquedo [%d] foi desligado.\n", toy->id); // Altere para o id do brinquedo
    pthread_exit(NULL);
}

// Essa função recebe como argumento informações e deve iniciar os brinquedos.
void open_toys(toy_args *args){
    // Cria N threads (N brinquedos)
    for (int i = 0; i < args->n; i++){
        pthread_create(&(args->toys[i]->threads), NULL, turn_on, args->toys[i]);
    }
}

// Desligando os brinquedos
void close_toys(){
    // Sua lógica aqui

    // Espera todas as threads finalizarem
    for (int i = 0; i < args->n; i++){
        pthread_join(toys[i], NULL);
    }
    // Libera a memória
    free(toys);
    // /Murta
}