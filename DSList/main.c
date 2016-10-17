#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "dslist.h"

#define arr_sz 5

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

    int i = 7777777;
    int arr[arr_sz] = {99, 56, 78, 99, 321};
    char *s = "Test";

    DLNode *node = dslist_append(NULL, (Pointer)s);

    assert( node != NULL && strcmp((char*) dslist_first(node)->data, s) == 0);

    assert( dslist_length(node) == 1);

    assert( dslist_reverse(node)->end->data == s );

    assert( dslist_append(node, (Pointer) i) == node);

    assert( dslist_last(node)->data == (Pointer) i);

    assert( dslist_length(node) == 2);

    assert( dslist_prepend(node, (Pointer) arr[0]) == node);

    assert( dslist_first(node)->data == (Pointer) arr[0]);

    assert( dslist_length(node) == 3);

    DLNode *cloned = dslist_copy(node);

    assert( cloned != NULL && dslist_length(cloned) == 3);

    DLNode *arrlst = dslist_prepend(NULL, (Pointer)arr[0]);
    for(int i = 1; i < arr_sz; i++){
        dslist_prepend(arrlst, (Pointer)arr[i]);
    }
    dslist_foreach(arrlst, print_arr, print_test);

    assert( dslist_length(arrlst) == arr_sz );

    DLList *arrend = dslist_find(arrlst, (Pointer)arr[arr_sz - 1]);

    assert( arrend != NULL );

    assert( dslist_position(arrlst, arrend) == 0 );

    DLList *arr0 = dslist_find_custom(arrlst, (Pointer)arr[0], find_it);

    assert(arrlst->end->data == arr0->data);

    printf("All tests passed!");

    return 0;
}
