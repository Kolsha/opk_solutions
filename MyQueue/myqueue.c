#include <stdlib.h>

#include "myqueue.h"

static int max(int a, int b){
    if(a > b){
        return a;
    }
    return b;
}

static int min(int a, int b){
    if(a > b){
        return b;
    }
    return a;
}

int queue_create(Queue *pqueue){

    if(pqueue == NULL){
        return 0;
    }
    pqueue->size = 0;
    pqueue->elements = NULL;
    pqueue->end = 0;
    pqueue->start = 0;
    pqueue->increment = 0;
    pqueue->count = 0;
    return 1;
}

void queue_destroy(Queue *pqueue){
    if(pqueue == NULL){
        return ;
    }
    free(pqueue->elements);
    free(pqueue);
}

int queue_tune(Queue *pqueue, size_t initial_size, size_t increment){
    if(pqueue == NULL || increment == 1){
        return 0;
    }
    pqueue->elements = (Pointer*) malloc(initial_size * sizeof(Pointer));
    if(pqueue->elements == NULL){
        pqueue->size = 0;
        return 0;
    }

    pqueue->increment = increment;
    pqueue->size = initial_size;
    pqueue->end = initial_size;
    pqueue->start = initial_size;
    pqueue->count = 0;

    return 1;
}

size_t queue_size(Queue *pqueue){
    if(pqueue == NULL){
        return 0;
    }
    return pqueue->count;
}

int queue_enqueue(Queue *pqueue, Pointer value){
    if(pqueue == NULL){
        return 0;
    }
    pqueue->count = pqueue->count + 1;
    pqueue->elements[pqueue->end - 1] = value;

    pqueue->end = pqueue->end - 1;

    if(pqueue->end == 0){
        pqueue->end = pqueue->size;
    }
    if(pqueue->end == pqueue->start){
        Pointer* tmp = (Pointer*) malloc((pqueue->size * pqueue->increment) * sizeof(Pointer));
        if(tmp == NULL){
            return 0;
        }

        for(size_t i = 0; i < pqueue->size; i++){
            if(i >= pqueue->end){
                tmp[i + pqueue->increment] = pqueue->elements[i];
            }
            else{
                tmp[i] = pqueue->elements[i];
            }
        }

        pqueue->size = pqueue->size + pqueue->increment;
        pqueue->end = pqueue->end + pqueue->increment;
        free(pqueue->elements);
        pqueue->elements = tmp;
    }
    return 1;
}

Pointer queue_peek(Queue *pqueue){
    if(pqueue == NULL || pqueue->count == 0){
        return NULL;
    }
    return pqueue->elements[pqueue->start - 1];
}

Pointer queue_dequeue(Queue *pqueue){
    Pointer tmp = queue_peek(pqueue);
    if(tmp != NULL){
        pqueue->start = pqueue->start - 1;
    }
    pqueue->count--;

    if(pqueue->start == 0){
        pqueue->start = pqueue->size;
    }

    if(pqueue->start == pqueue->end){
        pqueue->start = pqueue->size;
        pqueue->end = pqueue->size;
    }

    if(pqueue->count > 0){
        size_t size_to_count = pqueue->size / pqueue->count;
        if(size_to_count < 3){
            return tmp;
        }
        size_t  new_size = (size_to_count - 1) * pqueue->count;
        Pointer* elms = (Pointer*) malloc(new_size * sizeof(Pointer));
        if(elms == NULL){
            return tmp;
        }
        size_t from = min(pqueue->end, pqueue->start);
        size_t to= max(pqueue->end, pqueue->start);

        for(size_t i = 0; i < to; i++){
            elms[i] = pqueue->elements[i + from];
        }
        pqueue->start -= from;
        pqueue->end -= from;
        pqueue->size = new_size;
        free(pqueue->elements);
        pqueue->elements = elms;

    }
    return tmp;
}
