#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include "queue.c"
#define QUEUE_MAX 10

typedef struct Int Int;

struct Int {
    struct Int *prev; 
    struct Int *next;
    int inteiro;
};

Int *list_int;
Int item[QUEUE_MAX];
sem_t s1;

void init_mem(Int * item) {
    for (int i = 0; i < QUEUE_MAX; i++) {
        item[i].prev = NULL ;
        item[i].next = NULL ;
        item[i].inteiro = -1;
     }
}

Int * init_int(Int * item, int inteiro) {
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
            return primeiro;
        }
     }
    
    return primeiro;
}

Int * poe_elemento_no_fim_da_fila(int novo) {
    Int * elem = init_int(&item[0], novo);
    queue_append((queue_t **) &list_int, (queue_t *) elem);
    return elem;
}

void print_elem (void *ptr) {
   Int *elem = ptr;

   if (!elem)
      return ;

   elem->prev ? printf ("%d", elem->prev->inteiro) : printf ("*") ;
   printf ("<%d>", elem->inteiro) ;
   elem->next ? printf ("%d ", elem->next->inteiro) : printf ("*") ;
}

void preecher_list() {
    int novo;
    for(int i = 0; i < QUEUE_MAX; i++) {
        novo = random() % 100;
        poe_elemento_no_fim_da_fila(novo);
    } 
}

void * exec() {
    int velho, novo;
    velho = retira_primeiro_elemento_da_fila();
    novo = random() % 100;
    poe_elemento_no_fim_da_fila (novo);
    printf("Tira: %d Põe: %d Fila:", velho, novo);
    queue_print(" ", (queue_t *) list_int, print_elem);
}

void * thread_1() {
    while(1) {
        sleep(3);
        sem_wait(&s1);

        printf("Thread 1: \n");
        exec();

        sem_post(&s1);
    }   
}

void * thread_2() {
    while(1) {
        sleep(3);
        sem_wait(&s1);
    
        printf("Thread 2: \n");
        exec();

        sem_post(&s1);
    }   
}

int main() {
    pthread_t t1, t2;

    init_mem(&item[0]);
    preecher_list(); 
    queue_print("Lista inicial: ", (queue_t *) list_int, print_elem);

    sem_init(&s1, 1, 1);
    pthread_create(&t1,NULL,thread_1,NULL);
    pthread_create(&t2,NULL,thread_2,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);

}



