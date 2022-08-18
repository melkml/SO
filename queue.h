// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 1.4 -- Janeiro de 2022
// Definição e operações em uma fila genérica.

// ESTE ARQUIVO NÃO DEVE SER MODIFICADO - ELE SERÁ SOBRESCRITO NOS TESTES
#include <stdio.h>
#ifndef __QUEUE__
#define __QUEUE__

#ifndef NULL
#define NULL ((void *) 0)
#endif

//------------------------------------------------------------------------------
// estrutura de uma fila genérica, sem conteúdo definido.
// Veja um exemplo de uso desta estrutura em testafila.c

typedef struct queue_t
{
   struct queue_t *prev ;  // aponta para o elemento anterior na fila
   struct queue_t *next ;  // aponta para o elemento seguinte na fila
} queue_t ;

//------------------------------------------------------------------------------
// Conta o numero de elementos na fila
// Retorno: numero de elementos na fila

int queue_size (queue_t *queue) {
   // Fila vazia
    if(!queue) return 0;
    int size = 1;
    queue_t *aux;
    for(aux = queue; aux->next != queue; aux = aux->next) ++size;
    return size;

}

//------------------------------------------------------------------------------
// Percorre a fila e imprime na tela seu conteúdo. A impressão de cada
// elemento é feita por uma função externa, definida pelo programa que
// usa a biblioteca. Essa função deve ter o seguinte protótipo:
//
// void print_elem (void *ptr) ; // ptr aponta para o elemento a imprimir

void queue_print (char *name, queue_t *queue, void print_elem (void*)) {
    printf("%s [", name);
    // Fila vazia
    if(!queue) {
        print_elem(queue);
        puts("]");
        return;
    }
    queue_t *aux;
    for(aux = queue; aux->next != queue; aux = aux->next) print_elem(aux);
    print_elem(aux);
    puts("]");
    return;
}

//------------------------------------------------------------------------------
// Insere um elemento no final da fila.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - o elemento deve existir
// - o elemento nao deve estar em outra fila
// Retorno: 0 se sucesso, <0 se ocorreu algum erro

int queue_append (queue_t **queue, queue_t *elem) {
   if(!queue || !elem || !elem->next || !elem->prev) {
      return -1;
   }

   queue_t * q = *queue;

   elem->prev = q->prev;
   elem->next = *queue;
   q->prev = elem;

}

//------------------------------------------------------------------------------
// Remove o elemento indicado da fila, sem o destruir.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - a fila nao deve estar vazia
// - o elemento deve existir
// - o elemento deve pertencer a fila indicada
// Retorno: 0 se sucesso, <0 se ocorreu algum erro

int queue_remove (queue_t **queue, queue_t *elem) {
    if(!(*queue)) {
        return 0;
    }
    if(!elem) {
        return 0;
    }

    // Verifica se está na fila
    int achou = 0;
    queue_t *aux;
    for(aux = *queue; aux->next != *queue; aux = aux->next) {
        if(aux == elem) {
            achou = 1;
        }
    }

    if(!achou && elem->next != *queue) return 0;

    // Único elemento da fila
    if(elem->next == elem) {
        elem->next = NULL;
        elem->prev = NULL;
        *queue = NULL;
    } else {
        // Se remove o primeiro da fila
        if((*queue) == elem) *queue = elem->next;
        queue_t *prev = elem->prev;
        queue_t *next = elem->next;
        prev->next = next;
        next->prev = prev;
        elem->next = NULL;
        elem->prev = NULL;
    }
    return 1;

}

#endif
