#include <stdlib.h>

#include "mystack.h"

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

int stack_create(Stack *pstack){

    if(pstack == NULL){
        return 0;
    }
    pstack->size = 0;
    pstack->elements = NULL;
    pstack->inc = 0;
    pstack->pos = 0;
    return 1;
}

void stack_destroy(Stack *pstack){
    if(pstack == NULL){
        return ;
    }
    free(pstack->elements);
    free(pstack);
}

int stack_tune(Stack *pstack, size_t initial_size, size_t increment){
    if(pstack == NULL || increment == 1){
        return 0;
    }
    pstack->elements = (Pointer*) malloc(initial_size * sizeof(Pointer));
    if(pstack->elements == NULL){
        pstack->size = 0;
        return 0;
    }

    pstack->inc = increment;
    pstack->size = initial_size;
    pstack->pos = 0;

    return 1;
}

size_t stack_size(Stack *pstack){
    if(pstack == NULL){
        return 0;
    }
    return pstack->pos;
}

int stack_push(Stack *pstack, Pointer value){
    if(pstack == NULL){
        return 0;
    }


    if(pstack->pos >= pstack->start){
        Pointer* tmp = (Pointer*) malloc((pstack->size * pstack->inc) * sizeof(Pointer));
        if(tmp == NULL){
            return 0;
        }



        pstack->size *=  pstack->inc;
        free(pstack->elements);
        pstack->elements = tmp;
    }

    pstack->elements[pstack->pos] = value;

    pstack->pos++;
    return 1;
}

Pointer stack_peek(Stack *pstack){
    if(pstack == NULL || pstack->pos == 0){
        return NULL;
    }
    return pstack->elements[pstack->pos - 1];
}

Pointer queue_dequeue(Stack *pstack){
    Pointer tmp = stack_peek(pstack);
    if(tmp != NULL){
        pstack->start = pstack->start - 1;
    }
    pstack->count--;

    if(pstack->start == 0){
        pstack->start = pstack->size;
    }

    if(pstack->start == pstack->end){
        pstack->start = pstack->size;
        pstack->end = pstack->size;
    }

    if(pstack->count > 0){
        size_t size_to_count = pstack->size / pstack->count;
        if(size_to_count < 3){
            return tmp;
        }
        size_t  new_size = (size_to_count - 1) * pstack->count;
        Pointer* elms = (Pointer*) malloc(new_size * sizeof(Pointer));
        if(elms == NULL){
            return tmp;
        }
        size_t from = min(pstack->end, pstack->start);
        size_t to= max(pstack->end, pstack->start);

        for(size_t i = 0; i < to; i++){
            elms[i] = pstack->elements[i + from];
        }
        pstack->start -= from;
        pstack->end -= from;
        pstack->size = new_size;
        free(pstack->elements);
        pstack->elements = elms;

    }
    return tmp;
}
