#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "mystack.h"

#define ARR_SZ 50

int main()
{
    Stack ms;
    assert( stack_create(&ms) == 1);

    assert( ms.size == 0 && ms.pos == 0);

    assert( stack_tune(&ms, 2, 2) == 1);

    assert( ms.size == 2 && ms.inc == 2);

    char *a = "Test";
    assert( stack_push(&ms, (void*)a) == 1);

    assert( stack_size(&ms) == 1);

    assert( stack_peek(&ms) == a);

    assert( stack_pop(&ms) == a && stack_size(&ms) == 0);
    size_t arr[ARR_SZ] = {0};
    for(size_t i = 0; i < ARR_SZ; i++){
        arr[i] = i;
        assert( stack_push(&ms, (void*)&arr[i]) == 1);
    }

    void *t;
    while((t = stack_pop(&ms)) != NULL){
        size_t i = *(size_t*)t;
        printf("%d\n", i);
    }
    return 0;
}
