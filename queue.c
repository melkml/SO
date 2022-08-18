// Online C compiler to run C program online
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct queue_t
{
   struct queue_t *prev ;  // aponta para o elemento anterior na fila
   struct queue_t *next ;  // aponta para o elemento seguinte na fila
} queue_t ;

//------------------------------------------------------------------------------
// Conta o numero de elementos na fila
// Retorno: numero de elementos na fila

int queue_size (queue_t *queue) {
    if(!queue) {
        return 0;
    }
    
   int contador = 1;
   queue_t * aux = queue;

    while(aux->next != queue) {
      contador++;
      aux = aux->next;
   }

   return contador;
}

void queue_print (char *name, queue_t *queue, void print_elem (void*)) {
   printf("%s [ ", name);
    // Fila vazia
    if(!queue) {
        print_elem(queue);
        puts("]");
        return;
    }
    queue_t *aux = queue;

    while(aux->next != queue) {
      print_elem(aux);
      aux = aux->next;
    }
    print_elem(aux);
    puts("]");
}


int queue_append (queue_t **queue, queue_t *elem) {

     // Elemento inexistente
    if(!elem) {
      return -1;
    }
    // Elemento pertencente a outra fila
    if((elem->next != NULL) || (elem->prev != NULL)) {
      return -1;
    }  
    // Fila vazia
    
  if(!(*queue)) {
        elem->next = elem;
        elem->prev = elem;
        *queue = elem;
        return 0;
    }

    queue_t *aux = *queue;

    elem->prev = aux->prev;
    elem->next = aux;
   
    aux->prev->next = elem;
    aux->prev = elem;
    

    return 1;

}

int queue_remove (queue_t **queue, queue_t *elem) {
 if(!(*queue)) {
    return -1;
   }

if(!elem) {
   return -1;
}

    // Verificando se ta na fila
    int isInQueue = 0; 
    queue_t *aux = *queue;

   while(aux->next != *queue) {
      if(aux == elem) {
            isInQueue = 1;
        }
      aux = aux->next;
   }

    if(!isInQueue && elem->next != *queue) {
      return -1;
    }

    // Único elemento da fila
    if(elem->next == elem) {
        elem->next = NULL;
        elem->prev = NULL;
        *queue = NULL;
    } else {
        // Se remove o primeiro da fila
        if((*queue) == elem) {
         *queue = elem->next;
        }

        queue_t *prev = elem->prev;
        queue_t *next = elem->next;
        prev->next = next;
        next->prev = prev;
        elem->next = NULL;
        elem->prev = NULL;
    }
    return 0;
}



