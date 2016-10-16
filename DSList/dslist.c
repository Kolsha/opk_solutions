#include <stdlib.h>
#include "dslist.h"

static int check_list(DSList *list){
    if(list == NULL){
        //|| (list->next == NULL && list->prev == NULL)
        return 0;
    }
    return 1;
}

static DSList *head_of_list(DSList *list){
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

    DSList *nl = (DSList*) malloc(sizeof(DSList));
    if(nl == NULL){
        return NULL;
    }

    nl->data = data;
    nl->next = NULL;
    nl->prev = NULL;

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

    return head_of_list(list);
}

DSList *dslist_prepend(DSList *list, Pointer data){
    DSList *nl = (DSList*) malloc(sizeof(DSList));
    if(nl == NULL){
        return NULL;
    }

    nl->data = data;
    nl->next = NULL;
    nl->prev = NULL;

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
    DSList *nl = (DSList*) malloc(sizeof(DSList));
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
        free(list->data);
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

    return head_of_list(tmp);

}

DSList*dslist_remove_all(DSList*list, Pointer data){
    if(!check_list(list)){
        return NULL;
    }
    DSList *tmp = head_of_list(list) , *jump, *tmp2;

    while(tmp->next != NULL){
        tmp2 = tmp;
        jump = tmp->prev;
        tmp = tmp->next;
        if(tmp2->data == data){
            dslist_remove_raw(tmp2, 1);
        }
    }
    if(tmp2 != NULL && jump == NULL){
        jump = tmp2;
    }
    return head_of_list(jump);
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
    DSList *tmp = head_of_list(list), *tmp2;
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
    DSList *tmp = head_of_list(list);
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

    DSList *head = head_of_list(list), *tail = dslist_last(list);
    DSList *head_stat = head;

    if(head == tail){
        return head;
    }

    while(head != tail && (head != NULL) && (tail != NULL)){
        Pointer tmp = head->data;
        head->data = tail->data;
        tail->data = tmp;
        head = head->next;
        tail = tail->prev;
    }

    return head_stat;
}

DSList*dslist_concat(DSList*list1, DSList*list2){
    if(!check_list(list1)){
        return head_of_list(list2);
    }
    if(!check_list(list2)){
        return head_of_list(list1);
    }
    DSList *tail1 = dslist_last(list1);
    DSList *head1 = head_of_list(list1), *head2 = head_of_list(list2);
    tail1->next = head2;
    head2->prev = tail1;

    return head1;
}

void dslist_foreach(DSList*list,
                    void (*func)(Pointer data, Pointer user_data), Pointer user_data){
    if(!check_list(list) || func == NULL){
        return ;
    }
    DSList *head = head_of_list(list);
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
    DSList *head = head_of_list(haystack);
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
    DSList *head = head_of_list(list);
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

