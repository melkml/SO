#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>
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

struct timeval;


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

pthread_t t1, t2;

//! letra a
void letra_a() {
        pthread_create(&t1, NULL, exec, NULL);
        pthread_create(&t2, NULL, exec, NULL);
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
}

//! letra b
void letra_b() {
         int tempo = 0;

         while(tempo != 2) {
        if(tempo == 0) {
            pthread_create(&t1, NULL, exec, NULL);
            pthread_join(t1, NULL);
        }

        if(tempo == 1) {
            pthread_create(&t2, NULL, exec, NULL);
            pthread_join(t2, NULL);
        }

    tempo++;
        
    }
}


//! letra c
int flags[2];
int turn;

void * thread_1() {
    flags[0] = true;
    turn = 1;

    while (flags[1] && turn == 1) {}

    exec();
    flags[0] = false;
}

void * thread_2() {
    flags[1] = true;
    turn = 0;

    while (flags[0] && turn == 0) {}
    exec();
    flags[1] = true;
}

void letra_c() {


    pthread_create(&t1, NULL, thread_1, NULL);
    pthread_create(&t2, NULL, thread_2, NULL);
            
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}

//calcular tempo
void calcular_tempo(void funct()) {
    struct timeval tim;
             gettimeofday(&tim, NULL);
             double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
             funct();
             gettimeofday(&tim, NULL);
             double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
             printf("%.6lf seconds elapsed\n", t2-t1);
}

int main() {

    init_mem(&item[0]);
    preecher_list(); 

    calcular_tempo(letra_a);
    printf("\n");

    calcular_tempo(letra_b);
    printf("\n");

    calcular_tempo(letra_c);
    printf("\n");

    return 0;
}
