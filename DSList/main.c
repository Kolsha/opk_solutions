#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "dslist.h"

#define arr_sz 500

char *print_test = "Test it";
int find_it(Pointer a, Pointer b){
    if(a == b){
        return 0;
    }
    return 777;
}
void print_arr(Pointer data, Pointer user_data){
    printf("%d - %s \n", (int) data, (char*) user_data);
    assert(strcmp((char*) user_data, print_test) == 0);
}

int main(){

    int arr[arr_sz] = {99, 56, 78, 99, 321};
    char *s = "Test";

    DLList *list = dslist_append(NULL, (Pointer)s);

    assert( list != NULL && strcmp((char*) dslist_first(list)->data, s) == 0);

    assert( dslist_length(list) == 1);

    assert( dslist_reverse(list) == 1 );

    assert( list->last->data == s );

    assert( dslist_append(list, (Pointer) arr[0]) == list);

    assert( dslist_last(list)->data == (Pointer) arr[0]);

    assert( dslist_length(list) == 2);

    assert( dslist_prepend(list, (Pointer) arr[0]) == list);

    assert( dslist_first(list)->data == (Pointer) arr[0]);

    assert( dslist_length(list) == 3);

    DLList *cloned = dslist_copy(list);

    assert( cloned != NULL && dslist_length(cloned) == 3);

    DLList *arrlst = dslist_prepend(NULL, (Pointer)arr[0]);
    for(int i = 1; i < arr_sz; i++){
        dslist_prepend(arrlst, (Pointer)arr[i]);
    }
    dslist_foreach(arrlst, print_arr, print_test);

    assert( dslist_length(arrlst) == arr_sz );

    DLNode *arrend = dslist_find(arrlst, (Pointer)arr[arr_sz - 1]);

    assert( arrend != NULL );

    assert( dslist_position(arrlst, arrend) == 0 );

    DLNode *arr0 = dslist_find_custom(arrlst, (Pointer)arr[0], find_it);

    assert(arrlst->last->data == arr0->data);

    printf("All tests passed!");

    dslist_free(list);
    dslist_free(cloned);
    dslist_free(arrlst);

    return 0;
}
