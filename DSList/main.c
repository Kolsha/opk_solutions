#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "dslist.h"

int main(){
    char *s = "Test";
    int i = 7777777;
    DSList *list = dslist_append(NULL, (Pointer)s);
    DSList *list2 = dslist_append(NULL, (Pointer)s);

    assert( list != NULL && strcmp((char*) list->data, s) == 0);
    assert( dslist_length(list) == 1);
    assert( dslist_reverse(list)->data == s );
    assert( dslist_append(list, (Pointer) i) == list);
    assert( (int) dslist_last(list)->data == i);
    assert( dslist_length(list) == 2);

    DSList *list1 = dslist_prepend(list, (Pointer)i);
    assert( list1 != list);

    assert( dslist_concat(list1, list2) == list1);
    assert( dslist_position(list1, list2) == 3);

    assert( dslist_length(list1) == 4);
    printf("All tests passe!");

    return 0;
}
