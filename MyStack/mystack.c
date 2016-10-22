#include <stdlib.h>

#include "mystack.h"

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

    if(pstack->pos >= pstack->size){
        size_t new_size = (pstack->size * pstack->inc);
        Pointer* elms = (Pointer*) realloc(pstack->elements, new_size * sizeof(Pointer));

        if(elms == NULL){
            return 0;
        }

        pstack->elements = elms;
    }
    pstack->pos++;
    pstack->elements[pstack->pos - 1] = value;

    return pstack->elements[pstack->pos - 1] == value;
}

Pointer stack_peek(Stack *pstack){
    if(pstack == NULL || pstack->pos == 0){
        return NULL;
    }
    return pstack->elements[pstack->pos - 1];
}

Pointer stack_pop(Stack *pstack){

    if(pstack == NULL || pstack->pos == 0){
        return NULL;
    }

    Pointer tmp = pstack->elements[pstack->pos - 1];
    pstack->pos--;

    if(pstack->pos > 0){
        size_t size_to_count = pstack->size / pstack->pos;
        if(size_to_count < 3){
            return tmp;
        }
        size_t  new_size = (size_to_count - 1) * pstack->pos;
        Pointer* elms = (Pointer*) realloc(pstack->elements, new_size * sizeof(Pointer));

        if(elms == NULL){
            return tmp;
        }

        pstack->elements = elms;

    }

    return tmp;
}
