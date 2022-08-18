#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.c"
#define N 10

typedef struct Tarefa Tarefa;
typedef enum Estado Estado;

enum Estado {
    NOVA  = 0,
    PRONTA,
    EXECUTANDO,
    FINALIZADA, 
};

struct Tarefa {
    struct Tarefa *prev; 
    struct Tarefa *next;
    int id;
    int data_inicio;
    int data_fim;
    int duracao;
    int prioridade;
    Estado estado;
    int tempo_executado;
};

Tarefa item[N];
Tarefa *list_tarefa;

void print_elem (void *ptr) {
   Tarefa *elem = ptr;

   if (!elem)
      return ;

   elem->prev ? printf ("%d", elem->prev->id) : printf ("*") ;
   printf ("<%d>", elem->id) ;
   elem->next ? printf ("%d ", elem->next->id) : printf ("*") ;
}

void init_mem(Tarefa * item) {
    for (int i = 0; i < N; i++) {
        item[i].id = -1;
        item[i].prev = NULL ;
        item[i].next = NULL ;
        item[i].data_inicio = -1;
        item[i].data_fim = -1;
        item[i].duracao = -1;
        item[i].prioridade = -1; 
        item[i].estado = NOVA;
        item[i].tempo_executado = 0;
     }
}

void init_tarefas(Tarefa * item, int data_criacao, int duracao, int prioridade) {
      for (int i = 0; i < N; i++) {
        if(item[i].id == -1) {
            item[i].id = i;
            item[i].prev = NULL ;
            item[i].next = NULL ;
            item[i].data_inicio = data_criacao;
            item[i].data_fim = -1;
            item[i].duracao = duracao;
            item[i].prioridade = prioridade; 
            item[i].estado = NOVA;
            item[i].tempo_executado = 0;
            return;
        }
        
     }

     printf("Memoria de tarefas lotada!");
}

int main() {
    int duracao, data_criacao, prioridade;
    
    // inicializa memória
    init_mem(&item[0]);

    

    //Preenche a memoria com tarefas
    printf("Preencha a memoria de tarefas: \n");
       /*
        do {
            contador++;
            printf("Digite data de criação, duracao e prioridade da nova tarefa: \n");

            scanf("%d %d %d", &data_criacao, &duracao, &prioridade);
            
            init_tarefas(&item[0], data_criacao, duracao, prioridade);
        } while(contador != N - 1); */

        //teste
        init_tarefas(&item[0], 4, 3, 6);
        init_tarefas(&item[0], 7, 8, 9);
        init_tarefas(&item[0], 4, 7, 2);
        init_tarefas(&item[0], 1, 3, 5);
        init_tarefas(&item[0], 3, 2, 5);
        init_tarefas(&item[0], 6, 7, 8);
        init_tarefas(&item[0], 3, 2, 3);
        init_tarefas(&item[0], 0, 9, 2);
        init_tarefas(&item[0], 8, 4, 7);
        init_tarefas(&item[0], 10, 4, 6);

        int tempo = -1;
        Tarefa * tarefa_menor_duracao_atual = NULL;
        int qnt_tarefas_finalizadas = 0;

        printf("Tempo\n");
        
        //? Executando
        while(qnt_tarefas_finalizadas != N) {
            
            tempo++;
            printf("%d: \n", tempo);
           
           //? Adicionando tarefa na fila de acordo com o inicio da tarefa
            for(int i = 0; i < N; i++) {
                if(item[i].estado == NOVA && item[i].data_inicio == tempo) {
                    item[i].estado = PRONTA;
                    queue_append((queue_t **) &list_tarefa, (queue_t *) &item[i]);
                    queue_print("Adicionado elemento - Fila: ", (queue_t *) list_tarefa, print_elem);
                }
            }

            //? Se não tiver tarefa na fila, pula para o prox tempo de execução;
            if(!tarefa_menor_duracao_atual && !queue_size((queue_t *) list_tarefa)) {
                printf("Executando: Nenhuma ");
                queue_print("- Fila: ", (queue_t *) list_tarefa, print_elem);

                if(qnt_tarefas_finalizadas == N) {
                    printf("\nNão há mais tarefas a serem executas. FIM\n");
                }

                continue;
            }
        
            //? Se tem tarefa na fila de execução, continua o fluxo de execução
            //? Se tiver tarefa em execução, não realizar processo de escalonamento
            if(tarefa_menor_duracao_atual && tarefa_menor_duracao_atual->estado == EXECUTANDO) {
              if(tarefa_menor_duracao_atual->tempo_executado == tarefa_menor_duracao_atual->duracao) {
                    tarefa_menor_duracao_atual->data_fim = tempo;
                    tarefa_menor_duracao_atual->estado = FINALIZADA;
                    qnt_tarefas_finalizadas++;
                    printf("Execução de %d finalizada \n", tarefa_menor_duracao_atual->id);
                    tarefa_menor_duracao_atual = NULL;

               if(qnt_tarefas_finalizadas == N) {
                  printf("\nNão há mais tarefas a serem executas. FIM\n");
                  break;
               }
                    
              } else {
                tarefa_menor_duracao_atual->tempo_executado++;
              }

            } 
            
            if(!tarefa_menor_duracao_atual) {

                //?  Verificando menor duração da fila de espera (ESCALONAMENTO)
            Tarefa * aux = list_tarefa;
            for(int i = 0; i < queue_size((queue_t *) list_tarefa); i++) {
                
                //? No primeiro laço, atribui a primeira tarefa ao fluxo de execução
                if(!tarefa_menor_duracao_atual && i == 0) {
                    tarefa_menor_duracao_atual = aux;
                } 

                //? Verificando se a tarefa atual tem duração menor que a tarefa de menor duração atual
                if(aux->duracao < tarefa_menor_duracao_atual->duracao) {
                        tarefa_menor_duracao_atual = aux;
                }

                aux = aux->next;        
                
            }
            
            queue_remove((queue_t **) &list_tarefa, (queue_t *) tarefa_menor_duracao_atual);
            tarefa_menor_duracao_atual->estado = EXECUTANDO;
            tarefa_menor_duracao_atual->tempo_executado++;
            printf("Removido elemento: ");
            queue_print("- Fila: ", (queue_t *) list_tarefa, print_elem);
            }
            
            printf("Executando: %d \n", tarefa_menor_duracao_atual->id);
        }

    return 0;
}

