#ifndef D_S_LIST
#define D_S_LIST

typedef void *Pointer;

typedef struct tDLList {
    Pointer data;
    struct tDLList *next;
    struct tDLList *prev;
} DLList;

typedef struct tDLNode {
    size_t count;
    struct tDLList *first;
    struct tDLList *end;
} DLNode;


DLNode *dslist_append(DLNode *node, Pointer data);

DLNode *dslist_prepend(DLNode *node, Pointer data);

int dslist_insert(DLNode *node, DLList *sibling, Pointer data);

DLNode *dslist_remove(DLNode *node, Pointer data);

DLNode *dslist_remove_all(DLNode *node, Pointer data);

Pointer dslist_remove_next(DLNode *node, DLList *sibling);

void dslist_free(DLNode *node);

size_t dslist_length(DLNode *node);

DLNode *dslist_copy(DLNode *node);

DLNode *dslist_reverse(DLNode *node);

DLNode *dslist_concat(DLNode *node1, DLNode *node2);

void dslist_foreach(DLNode*node,
                    void (*func)(Pointer data, Pointer user_data), Pointer user_data);

DLList *dslist_last(DLNode *node);

DLList *dslist_first(DLNode *node);

DLList *dslist_nth(DLNode *node, int n);

DLList *dslist_find(DLNode *haystack, Pointer needle);

DLList *dslist_find_custom(DLNode *haystack, Pointer needle,
                           int (*compare_func)(Pointer a, Pointer b));

int dslist_position(DLNode *node, DLList *el);

#endif
