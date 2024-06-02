#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <defs.h>

// Você pode declarar novas funções aqui
void create_clients(client_args *args);
void* enjoy(void *arg);

/********************************
 *          ATENÇÃO             *
 *   NÃO EDITAR ESSAS FUNÇÕES   *
 *          ATENÇÃO             *
 *******************************/
void open_gate(client_args *args);
void close_gate();
void queue_enter(client_t *self);
#endif
