#include <stdlib.h>
#include "dslist.h"

static int check_list(DSList *list){
    if(list == NULL){
        //|| (list->next == NULL && list->prev == NULL)
        return 0;
    }
    return 1;
}

static void swap_data(DSList *a, DSList *b){
    Pointer tmp = a->data;
    a->data = b->data;
    b->data = tmp;
}

static void copy_data(DSList *in, DSList *source){

    in->data = source->data;

}

static void free_data(DSList *list){
    if(check_list(list))
    {
        free(list->data);
    }
}

DSList *dslist_first(DSList *list){

    if(!check_list(list)){
        return NULL;
    }
    DSList *tmp = list , *tmp2;
    while(tmp != NULL){
        tmp2 = tmp;

        tmp = tmp->prev;
    }
    return tmp2;
}

static DSList *empty_list(){

    DSList *nl = (DSList*) malloc(sizeof(DSList));
    if(nl == NULL){
        return NULL;
    }

    nl->data = NULL;
    nl->next = NULL;
    nl->prev = NULL;
    return nl;
}

DSList*dslist_last(DSList*list){
    if(!check_list(list)){
        return NULL;
    }
    DSList *tmp = list , *tmp2;
    while(tmp != NULL){
        tmp2 = tmp;
        tmp = tmp->next;
    }
    return tmp2;
}

DSList *dslist_append(DSList *list, Pointer data){

    DSList *nl = empty_list();
    if(nl == NULL){
        return NULL;
    }
    nl->data = data;

    if(list == NULL){
        return nl;
    }

    DSList *tmp = list , *tmp2;
    while(tmp != NULL){
        tmp2 = tmp;
        tmp = tmp->next;
    }

    nl->prev = tmp2;
    tmp2->next = nl;

    return dslist_first(list);
}

DSList *dslist_prepend(DSList *list, Pointer data){

    DSList *nl = empty_list();
    if(nl == NULL){
        return NULL;
    }
    nl->data = data;

    if(list == NULL){
        return nl;
    }

    DSList *tmp = list , *tmp2;
    while(tmp != NULL){
        tmp2 = tmp;
        tmp = tmp->prev;
    }

    nl->next = tmp2;
    tmp2->prev = nl;

    return nl;
}

int dslist_insert(DSList *sibling, Pointer data){
    if(!check_list(sibling)){
        return 0;
    }
    DSList *nl = empty_list();
    if(nl == NULL){
        return 0;
    }

    nl->data = data;

    DSList *tmp = sibling->next;
    sibling->next = nl;
    nl->prev = sibling;
    nl->next = tmp;
    return 1;

}

static DSList *dslist_remove_raw(DSList*list, int freeadata){
    if(!check_list(list)){
        return NULL;
    }
    DSList *prev = list->prev, *next = list->next, *tmp = NULL;
    if(freeadata == 1){
        free_data(list);
    }
    free(list);

    if(next != NULL){
        next->prev = prev;
        tmp = next;
    }
    if(prev != NULL){
        prev->next = next;
        tmp = prev;
    }
    return tmp;
}

DSList *dslist_remove(DSList*list, Pointer data){

    DSList *tmp = dslist_remove_raw(list, 1);
    if(tmp == NULL){
        return NULL;
    }
    free(data);

    return dslist_first(tmp);

}

DSList*dslist_remove_all(DSList*list, Pointer data){
    if(!check_list(list)){
        return NULL;
    }
    DSList *tmp = dslist_first(list) , *jump, *tmp2;

    while(tmp->next != NULL){
        tmp2 = tmp;
        jump = tmp->prev;
        tmp = tmp->next;
        if(tmp2->data == data)
        {
            dslist_remove_raw(tmp2, 1);
        }
    }
    if(tmp2 != NULL && jump == NULL){
        jump = tmp2;
    }
    return dslist_first(jump);
}

Pointer dslist_remove_next(DSList*sibling){
    if(!check_list(sibling) ||  !check_list(sibling->next)){
        return NULL;
    }
    Pointer data= sibling->next->data;
    dslist_remove_raw(sibling->next, 0);
    return data;
}

void dslist_free(DSList*list){
    if(!check_list(list)){
        return ;
    }
    DSList *tmp = dslist_first(list), *tmp2;
    while(tmp->next != NULL){
        tmp2 = tmp;
        tmp = tmp->next;
        dslist_remove_raw(tmp2, 1);
    }
}

size_t dslist_length(DSList*list){
    if(!check_list(list)){
        return 0;
    }
    size_t length = 1;
    DSList *tmp = dslist_first(list);
    while(tmp->next != NULL){
        tmp = tmp->next;
        length++;
    }
    return length;

}

DSList*dslist_reverse(DSList*list){
    if(!check_list(list)){
        return NULL;
    }

    DSList *head = dslist_first(list), *tail = dslist_last(list);
    DSList *head_stat = head;

    if(head == tail){
        return head;
    }

    while(head != tail && (head != NULL) && (tail != NULL)){
        swap_data(head, tail);
        head = head->next;
        tail = tail->prev;
    }

    return head_stat;
}

DSList*dslist_concat(DSList*list1, DSList*list2){
    if(!check_list(list1)){
        return dslist_first(list2);
    }
    if(!check_list(list2)){
        return dslist_first(list1);
    }
    DSList *tail1 = dslist_last(list1);
    DSList *head1 = dslist_first(list1), *head2 = dslist_first(list2);
    tail1->next = head2;
    head2->prev = tail1;

    return head1;
}

void dslist_foreach(DSList*list,
                    void (*func)(Pointer data, Pointer user_data), Pointer user_data){
    if(!check_list(list) || func == NULL){
        return ;
    }
    DSList *head = dslist_first(list);
    while(head != NULL){
        func(head->data, user_data);
        head = head->next;
    }
}

DSList*dslist_find_custom(DSList*haystack, Pointer needle,
                          int (*compare_func)(Pointer a, Pointer b)){
    if(!check_list(haystack) || compare_func == NULL){
        return NULL;
    }
    DSList *head = dslist_first(haystack);
    while(head != NULL){
        if(!compare_func(head->data, needle)){
            return head;
        }
        head = head->next;
    }
    return NULL;
}

int dslist_position(DSList*list, DSList*el){
    if(!check_list(list) || !check_list(el)){
        return -1;
    }
    DSList *head = dslist_first(list);
    int pos = 0;
    while(head != NULL){
        if(head == el){
            return pos;
        }
        head = head->next;
        pos++;
    }
    return -1;

}

DSList*dslist_copy(DSList*list){
    if(!check_list(list)){
        return NULL;
    }
    DSList *head1 = dslist_first(list);
    DSList *head2 = empty_list();
    DSList *head2_stat = head2;
    if(head2 == NULL || head1 == NULL){
        return NULL;
    }
    while(head1 != NULL){
        copy_data(head2, head1);

        if(head1->next != NULL){
            DSList *tmp = head2;
            head2 = empty_list();
            if(head2 == NULL){
                return tmp;
            }
            tmp->next = head2;
            head2->prev = tmp;
        }
        head1 = head1->next;
    }
    return head2_stat;
}

DSList*dslist_find(DSList*haystack, Pointer needle){
    if(!check_list(haystack)){
        return NULL;
    }
    DSList *head = dslist_first(haystack);
    while(head != NULL){
        if(head->data == needle){
            return head;
        }
        head = head->next;
    }
    return NULL;

}

DSList*dslist_nth(DSList*list, int n){

    if(!check_list(list)){
        return NULL;
    }
    DSList *runner;
    if(n > 0)
    {
        runner = dslist_first(list);
    }
    else{
        runner = dslist_last(list);
    }

    if(runner == list && list->prev == NULL && list->next == NULL){
        return (abs(n) == 1) ? list : NULL;
    }
    int pos = 1;
    while(runner != NULL){
        if(abs(n) == pos){
            return runner;
        }
        if(n > 0)
        {
            runner = runner->next;
        }
        else{
            runner = runner->prev;
        }
        pos++;

    }
    return NULL;
}
