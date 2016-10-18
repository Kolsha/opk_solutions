#ifndef D_S_LIST
#define D_S_LIST

typedef void *Pointer;

typedef struct tDLNode {
    Pointer data;
    struct tDLNode *next;
    struct tDLNode *prev;
} DLNode;

typedef struct tDLList {
    size_t count;
    struct tDLNode *first;
    struct tDLNode *last;
} DLList;

int dslist_insert(DLList *list, DLNode *sibling, Pointer data);

int dslist_remove(DLList *list, Pointer data);

int dslist_remove_all(DLList *list, Pointer data);

int dslist_reverse(DLList *list);

int dslist_position(DLList *list, DLNode *el);


size_t dslist_length(DLList *list);


Pointer dslist_remove_next(DLList *list, DLNode *sibling);


DLNode *dslist_last(DLList *list);

DLNode *dslist_first(DLList *list);

DLNode *dslist_nth(DLList *list, int n);

DLNode *dslist_find(DLList *haystack, Pointer needle);

DLNode *dslist_find_custom(DLList *haystack, Pointer needle,
                           int (*compare_func)(Pointer a, Pointer b));


DLList *dslist_append(DLList *list, Pointer data);

DLList *dslist_prepend(DLList *list, Pointer data);

DLList *dslist_copy(DLList *list);

DLList *dslist_concat(DLList *list1, DLList *list2);


void dslist_free(DLList *list);

void dslist_foreach(DLList *list,
                    void (*func)(Pointer data, Pointer user_data), Pointer user_data);

#endif
