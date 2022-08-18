#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include "queue.c"
#define QUEUE_MAX 5

typedef struct Int Int;

struct Int {
    struct Int *prev; 
    struct Int *next;
    int inteiro;
};

Int item[QUEUE_MAX];
Int * list_int;
sem_t s_vaga, s_buffer, s_item;

void print_elem (void *ptr) {
   Int *elem = ptr;

   if (!elem)
      return ;

   elem->prev ? printf ("%d", elem->prev->inteiro) : printf ("*") ;
   printf ("<%d>", elem->inteiro) ;
   elem->next ? printf ("%d ", elem->next->inteiro) : printf ("*") ;
}

void init_mem() {
    for (int i = 0; i < QUEUE_MAX; i++) {
        item[i].prev = NULL ;
        item[i].next = NULL ;
        item[i].inteiro = -1;
     }
}

Int * init_int(int inteiro) {
    for (int i = 0; i < QUEUE_MAX; i++) {
        if(item[i].inteiro == -1) {
            item[i].inteiro = inteiro;
            return &item[i];
        }
     }

    printf("Memoria de tarefas lotada!");
    return NULL;
}

int retira_primeiro_elemento_da_fila() {
    int primeiro = list_int->inteiro;
    queue_remove((queue_t **) &list_int, (queue_t *) list_int);

     for (int i = 0; i < QUEUE_MAX; i++) {
        if(item[i].inteiro == primeiro) {
            item[i].inteiro = -1;
            item[i].prev = NULL;
            item[i].next = NULL;
            return primeiro;
        }
     }
    
    return primeiro;
}

Int * poe_elemento_no_fim_da_fila(int novo) {
    Int * elem = init_int(novo);
    queue_append((queue_t **) &list_int, (queue_t *) elem);
    return elem;
}


void * produtor () {
    while(true) {
        sleep(2);
        sem_wait(&s_vaga);
        sem_wait(&s_buffer);
        int item = random() % 100;
        poe_elemento_no_fim_da_fila(item);
        printf("Inserido: %d\n", item);
        sem_post(&s_buffer);
        sem_post(&s_item);
        sleep(2);
    }
        
        
}

void * consumidor () {
    while(true) {
        sleep(2);
        sem_wait(&s_item);
        sem_wait(&s_buffer);
        int removido = retira_primeiro_elemento_da_fila();
        printf("Removido: %d\n", removido);
        sem_post(&s_buffer);
        sem_post(&s_vaga);
        sleep(2);   
    }
        
}


int main() {

    pthread_t produ_1, produ_2, produ_3, consu_1, consu_2;

    init_mem();
    queue_print("Lista: ", (queue_t *) list_int, print_elem);

    sem_init(&s_vaga, 0, QUEUE_MAX);
    sem_init(&s_buffer, 0, 1);
    sem_init(&s_item, 0, 0);

    pthread_create(&produ_1, NULL, produtor, NULL);
    pthread_create(&produ_2, NULL, produtor, NULL);
    pthread_create(&produ_3, NULL, produtor, NULL);

    pthread_create(&consu_1, NULL, consumidor, NULL);
    pthread_create(&consu_2, NULL, consumidor, NULL);

    pthread_join(produ_1,NULL);
    pthread_join(produ_2,NULL);
    pthread_join(produ_3,NULL);
    pthread_join(consu_1,NULL);
    pthread_join(consu_2,NULL);

    return 0;
}