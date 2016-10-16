#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "myqueue.h"


char *GetTextFromConsole(){
    size_t c = 1;
    int key;
    char *s, *s1;
    s = (char*) malloc((c) * sizeof(char));
    if (s == NULL) {
        return NULL;
    }
    s[c-1] = '\0';
    while ((key = getchar ()) != 10) {
        s[c-1] = key;
        c++;
        s1 = (char*) realloc(s, (c + 1) * sizeof(char));
        if (s1 == NULL){
            break;
        }
        s = s1;
        s[c-1] = '\0';
    }
    s[c] = '\0';
    return s;
}


int main()
{


    //Pointer *el = malloc(5 * sizeof(Pointer));

    //return ;

    Queue mq;
    assert( queue_create(&mq) == 1);
    assert( mq.size == 0 && mq.count == 0);

    assert( queue_tune(&mq, 10, 1) == 1);

    assert( mq.size == 10 && mq.increment == 1);

    char *a = "Test";
    assert( queue_enqueue(&mq, (void*)a) == 1);

    assert( queue_size(&mq) == 1);

    assert( queue_peek(&mq) == a);

    assert( queue_dequeue(&mq) == a && queue_size(&mq) == 0);

    while(1 <3)
    {
        printf("Write text(0 - exit): ");
        a = GetTextFromConsole();
        if(strcmp(a, "0") == 0){
            break;
        }
        assert( queue_enqueue(&mq, (void*)a) == 1);
    }
    printf("\n");
    void *t;
    while((t = queue_dequeue(&mq)) != NULL){
        a = (char*)t;
        printf("%s\n", a);
    }

    return 0;
}
