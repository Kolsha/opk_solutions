#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "myqueue.h"

#define ARR_SZ 500

int main()
{
    Queue mq;
    assert( queue_create(&mq) == 1);

    assert( mq.size == 0 && mq.count == 0);

    assert( queue_tune(&mq, 2, 2) == 1);

    assert( mq.size == 2 && mq.increment == 2);

    char *a = "Test";
    assert( queue_enqueue(&mq, (void*)a) == 1);

    assert( queue_size(&mq) == 1);

    assert( queue_peek(&mq) == a);

    assert( queue_dequeue(&mq) == a && queue_size(&mq) == 0);
    size_t arr[ARR_SZ] = {0};
    for(size_t i = 0; i < ARR_SZ; i++){
        arr[i] = i;
        assert( queue_enqueue(&mq, (void*)&arr[i]) == 1);
    }

    void *t;
    while((t = queue_dequeue(&mq)) != NULL){
        size_t i = *(size_t*)t;
        printf("%d\n", i);
    }

    return 0;
}
