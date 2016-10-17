#include <stdlib.h>
#include "dslist.h"

static int check_list(void *list){
    return list != NULL;
}

static void swap_data(DLList *a, DLList *b){
    Pointer tmp = a->data;
    a->data = b->data;
    b->data = tmp;
}

static void free_data(DLList *list){
    if(check_list(list))
    {
        free(list->data);
    }
}

DLList *dslist_first(DLNode *list){
    if(!check_list(list) || list->count == 0){
        return NULL;
    }
    return list->first;
}

static DLNode *empty_node(DLList *list){

    DLNode *nn = (DLNode*) malloc(sizeof(DLNode));
    if(nn == NULL){
        return NULL;
    }

    nn->count = (list != NULL) ? 1 : 0;
    nn->end = list;
    nn->first = list;
    return nn;
}

static DLList *empty_list(){

    DLList *nl = (DLList*) malloc(sizeof(DLList));
    if(nl == NULL){
        return NULL;
    }

    nl->data = NULL;
    nl->next = NULL;
    nl->prev = NULL;
    return nl;
}

DLList* dslist_last(DLNode *list){
    if(!check_list(list) || list->count == 0){
        return NULL;
    }
    return list->end;
}

DLNode *dslist_append(DLNode *node, Pointer data){
    DLList *nl = empty_list();
    if(nl == NULL){
        return NULL;
    }
    nl->data = data;

    if(node == NULL){
        DLNode *tmp = empty_node(nl);
        if(tmp == NULL){
            free(nl);
            return NULL;
        }
        return tmp;
    }
    if(node->count > 0)
    {
        nl->prev = node->end;
        node->end->next = nl;
    }
    else{
        node->first = nl;
    }
    node->end = nl;
    node->count++;

    return node;
}

DLNode *dslist_prepend(DLNode *node, Pointer data){
    DLList *nl = empty_list();
    if(nl == NULL){
        return NULL;
    }
    nl->data = data;

    if(node == NULL){
        DLNode *tmp = empty_node(nl);
        if(tmp == NULL){
            free(nl);
            return NULL;
        }
        return tmp;
    }
    if(node->count > 0)
    {
        nl->next = node->first;
        node->first->prev = nl;
    }
    else{
        node->end = nl;
    }
    node->first = nl;
    node->count++;

    return node;
}

int dslist_insert(DLNode *node, DLList *sibling, Pointer data){

    if(!check_list(node)){
        return 0;
    }
    DLList *nl = empty_list();
    if(nl == NULL){
        return 0;
    }
    nl->data = data;
    if(sibling == NULL || node->end == sibling){
        return dslist_prepend(node, data) != NULL;
    }

    nl->next = sibling->next;
    sibling->next = nl;
    nl->prev = sibling;

    return 1;

}

static void dslist_remove_raw(DLList *list, int freeadata){
    if(!check_list(list)){
        return ;
    }
    DLList *prev = list->prev;
    DLList *next = list->next;
    if(freeadata == 1){
        free_data(list);
    }
    free(list);

    if(next != NULL){
        next->prev = prev;
    }
    if(prev != NULL){
        prev->next = next;
    }
}

static DLNode *dslist_remove_few(DLNode *node, Pointer data, int count){
    if(!check_list(node) || node->count < 1){
        return 0;
    }
    DLList *runner = node->first;
    int pos = 0;
    while(runner != NULL){
        if(runner->data == data){
            if(node->first == runner){
                node->first = runner->next;
            }
            if(node->end == runner){
                node->end = runner->prev;
            }
            dslist_remove_raw(runner, 1);
            node->count--;
            pos++;
            if((pos >= count && count > 0) || node->count < 1)
            {
                return node;
            }
        }
    }
    return node;
}

DLNode *dslist_remove(DLNode *node, Pointer data){
    return dslist_remove_few(node, data, 1);
}

DLNode *dslist_remove_all(DLNode *node, Pointer data){
    return dslist_remove_few(node, data, -1);
}

Pointer dslist_remove_next(DLNode *node, DLList *sibling){
    if(!check_list(sibling) ||  !check_list(sibling->next)
            || !check_list(node) || node->count < 1){
        return NULL;
    }
    Pointer data= sibling->next->data;
    if(node->first == sibling){
        node->first = sibling->next;
    }
    if(node->end == sibling){
        node->end = sibling->prev;
    }
    dslist_remove_raw(sibling->next, 0);
    node->count--;
    return data;
}

void dslist_free(DLNode *node){
    if(!check_list(node)){
        return ;
    }
    DLList *tmp = node->first, *tmp2;
    while(tmp->next != NULL){
        tmp2 = tmp;
        tmp = tmp->next;
        dslist_remove_raw(tmp2, 1);
    }
    free(node);
}

size_t dslist_length(DLNode *node){
    if(!check_list(node)){
        return 0;
    }
    return node->count;
}

DLNode *dslist_reverse(DLNode *node){
    if(!check_list(node)){
        return NULL;
    }

    DLList *head = node->first;
    DLList *tail = node->end;

    if(head == tail){
        return node;
    }

    while(head != tail && (head != NULL) && (tail != NULL)){
        swap_data(head, tail);
        head = head->next;
        tail = tail->prev;
    }

    return node;
}

DLNode *dslist_concat(DLNode *node1, DLNode *node2){
    if(!check_list(node1)){
        return node2;
    }
    if(!check_list(node2)){
        return node1;
    }
    DLList *tail1 = node1->end;
    DLList *head2 = node2->first;

    node1->count += node2->count;
    tail1->next = head2;
    head2->prev = tail1;

    return node1;
}

void dslist_foreach(DLNode *node,
                    void (*func)(Pointer data, Pointer user_data), Pointer user_data){
    if(!check_list(node) || func == NULL){
        return ;
    }
    DLList *head = node->first;
    while(head != NULL){
        func(head->data, user_data);
        head = head->next;
    }
}

DLList *dslist_find_custom(DLNode *haystack, Pointer needle,
                           int (*compare_func)(Pointer a, Pointer b)){
    if(!check_list(haystack) || compare_func == NULL){
        return NULL;
    }
    DLList *head = dslist_first(haystack);
    while(head != NULL){
        if(!compare_func(head->data, needle)){
            return head;
        }
        head = head->next;
    }
    return NULL;
}

int dslist_position(DLNode *node, DLList *el){
    if(!check_list(node) || !check_list(el)){
        return -1;
    }
    DLList *head = node->first;
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

static void foreach_for_copy(Pointer data, Pointer user_data){
    if(!check_list(data) || !check_list(user_data)){
        return ;
    }
    dslist_append((DLNode*) user_data, data);

}

DLNode *dslist_copy(DLNode *node){

    DLNode* nn = empty_node(NULL);
    if(nn == NULL){
        return NULL;
    }
    dslist_foreach(node, foreach_for_copy, nn);
    return nn;
}

DLList *dslist_find(DLNode *haystack, Pointer needle){
    if(!check_list(haystack)){
        return NULL;
    }
    DLList *head = haystack->first;
    while(head != NULL){
        if(head->data == needle){
            return head;
        }
        head = head->next;
    }
    return NULL;

}

DLList *dslist_nth(DLNode *node, int n){

    if(!check_list(node)){
        return NULL;
    }
    DLList *runner;
    if(n > 0)
    {
        runner = node->first;
    }
    else{
        runner = node->end;
    }

    if(node->first == node->end){
        return (abs(n) == 1) ? runner : NULL;
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
