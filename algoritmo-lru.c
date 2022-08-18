#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "queue.c"
#define N 5

typedef struct Page Page;

struct Page {
    struct Page *prev; 
    struct Page *next;
    int id;
};

Page item[N];
Page * list_page;

void print_elem (void *ptr) {
   Page *elem = ptr;

   if (!elem)
      return ;

   printf("%d ", elem->id) ;
  
}

void init_mem() {
    for (int i = 0; i < N; i++) {
        item[i].id = -1;
        item[i].prev = NULL;
        item[i].next = NULL;
     }
}

void free_page_mem(int id) {
      for (int i = 0; i < N; i++) {
        if(id == item[i].id) {
            item[i].id = -1;
            break;
        }
     }
}

Page * init_page(Page * item, int pageId) {
      for (int i = 0; i < N; i++) {
        if(item[i].id == -1) {
            item[i].id = pageId;
            item[i].prev = NULL ;
            item[i].next = NULL ;
            return &item[i];
        }
      }

     printf("Memoria de tarefas lotada!");
     return NULL;
}


// bool check_in_list(Page ** list, int pageId) {
//  bool isInQueue = false; 
//  Page *aux = *list;

//    while(aux->next != *list) {
//       if(aux->id == pageId) {
//             isInQueue = true;
//         }
//       aux = aux->next;
//    }

//     return isInQueue;
// }


Page * get_page_by_id(Page ** list, int pageId) {

// FIla vazia
if(!(*list)) {
    return NULL;
}

if(pageId < 0) {
    return NULL;
}

Page *aux = *list;

//Fila com um elemento
if(aux->next == aux) {
    if(aux->id == pageId) {
        return aux;
    }

    return NULL;
}

do {
      if(aux->id == pageId) {
            return aux;
        }
        
      aux = aux->next;
      
   } while((aux != *list));

    return NULL;
}

void insert_page(int id) {
    Page * page =  get_page_by_id(&list_page, id);

    if(page) {
        queue_remove((queue_t **) &list_page, (queue_t *) page);
        queue_append((queue_t **) &list_page, (queue_t *) page);
    } else {
        int size = queue_size((queue_t *) list_page);

        if(size == N) {
            int removido = list_page->id;
            free_page_mem(list_page->id);
            queue_remove((queue_t **) &list_page, (queue_t *) list_page);
            printf("Page %d removida do cache.\n", removido);
        }

        page = init_page(&item[0], id);

        if(!page) {
            return;
        }

        queue_append((queue_t **) &list_page, (queue_t *) page);
    }

    printf("Page %d usada. ", page->id);
    queue_print("Cache:", (queue_t *) list_page, print_elem);
}

int main() {
    init_mem();

    while(true) {
        
        insert_page(rand() % 10);
         sleep(1);

        // printf("Memoria: [ ");
        // for (int i = 0; i < N; i++) {
        //      printf("%d ", item[i].id);
        // }
        // printf("]\n");
    
    }
   
       
    

   

    

       
    


    return 0;
}

