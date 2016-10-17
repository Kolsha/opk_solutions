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
    DSList *list = dslist_append(NULL, (Pointer)s);
    DSList *list2 = dslist_append(NULL, (Pointer)s);

    assert( list != NULL && strcmp((char*) list->data, s) == 0);
    assert( dslist_length(list) == 1);
    assert( dslist_reverse(list)->data == s );
    assert( dslist_append(list, (Pointer) i) == list);
    assert( dslist_last(list)->data == (Pointer) i);
    assert( dslist_length(list) == 2);

    DSList *list1 = dslist_prepend(list, (Pointer)i);
    assert( list1 != list);

    assert( dslist_concat(list1, list2) == list1);
    assert( dslist_position(list1, list2) == 3);

    assert( dslist_length(list1) == 4);

    DSList *copy = dslist_copy(list1);
    assert( dslist_length(copy) == 4);
    assert( dslist_find(list1, (Pointer)s) == list);

    assert( dslist_insert(copy, (Pointer)arr[0]) == 1);
    DSList *arr0 = dslist_find_custom(copy, (Pointer)arr[0], find_it);
    assert( dslist_position(copy, arr0) == 1);

    DSList *arrlst = dslist_prepend(NULL, (Pointer)arr[0]);
    for(int i = 1; i < arr_sz; i++){
        arrlst = dslist_prepend(arrlst, (Pointer)arr[i]);
    }
    dslist_foreach(arrlst, print_arr, print_test);
    assert( dslist_nth(arrlst, 1)->data == (Pointer)arr[arr_sz - 1]);

    printf("All tests passed!");

    return 0;
}
