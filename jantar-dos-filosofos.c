#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
#include<stdbool.h>
#include <unistd.h>
#define NUM_FILOSOFO_AND_COLHER 5

sem_t colher[NUM_FILOSOFO_AND_COLHER];
int indexFilosofos[NUM_FILOSOFO_AND_COLHER] = {0, 1, 2, 3, 4};

int colher_esquerda(int filosofo) {
    return filosofo;
}

int colher_direita(int filosofo) {
    return (filosofo + 1) % NUM_FILOSOFO_AND_COLHER;
}

void pensar(int filosofo) {
    printf("Filosofo %d está pensando.\n", filosofo);
}

void comer(int filosofo) {
    printf("Filosofo %d está comendo.\n", filosofo);
}

void pegar_hashi(int filosofo) {

    int colherDireita = colher_direita(filosofo);
    int colherEsquerda = colher_esquerda(filosofo);

   if(filosofo == 4) {
    sem_wait(&colher[colherDireita]);
    sem_wait(&colher[colherEsquerda]);
   } else {
    sem_wait(&colher[colherEsquerda]);
    sem_wait(&colher[colherDireita]);
   }

   printf("Filosofo %d pegou os hashis %d e %d.\n", filosofo, colherDireita, colherEsquerda);
}

void soltar_hashi(int filosofo) {
    int colherDireita = colher_direita(filosofo);
    int colherEsquerda = colher_esquerda(filosofo);

    sem_post(&colher[colher_direita(filosofo)]);
    sem_post(&colher[colher_esquerda(filosofo)]);

     printf("Filosofo %d soltou os hashis %d e %d.\n", filosofo, colherDireita, colherEsquerda);
}

void * filosofo(void * filosofo) {
    int * i = (int *) filosofo;

    while (true) {
        pensar(*i);
        sleep(3);
        pegar_hashi(*i);
        sleep(3);
        comer(*i);
        sleep(3);
        soltar_hashi(*i);
        sleep(3);
    }
      
}

int main() {
   int i;
   pthread_t f[NUM_FILOSOFO_AND_COLHER];

   for(int i = 0; i < NUM_FILOSOFO_AND_COLHER; i++) {
    sem_init(&colher[i], 0, 1);
   }

   for(int i = 0; i < NUM_FILOSOFO_AND_COLHER; i++) {
    pthread_create(&f[i], NULL, filosofo, (void *) &indexFilosofos[i]);
   }

    for(int i = 0; i < NUM_FILOSOFO_AND_COLHER; i++) {
    pthread_join(f[i], NULL);
   }

   

   return(0);
}