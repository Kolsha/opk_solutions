#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include <stdlib.h>

typedef void *Pointer;
typedef unsigned (*HashFunction)(char *key, size_t ht_size);
typedef void (*Destructor)(Pointer);

typedef struct List{
    char *key;
    Pointer data;
    struct List *next;
} List;

typedef struct HashTable {
    size_t size;
    size_t count;
    List **table;
    HashFunction hashfunc;
    Destructor dtor;
} HashTable;

typedef int (*TraverseFunc)(char *key, Pointer data, Pointer extra_data);

int ht_init(HashTable *ht, size_t size, HashFunction hf, Destructor dtor);

HashTable *create_HashTable(size_t size, HashFunction hf, Destructor dtor);

void ht_destroy(HashTable *ht);

Pointer ht_set(HashTable *ht, char *key, Pointer data);

Pointer ht_get(HashTable *ht, char *key);

int ht_has(HashTable *ht, char *key);

int ht_has_strict(HashTable *ht, char *key);

void ht_delete(HashTable *ht, char *key);

void ht_traverse(HashTable *ht, TraverseFunc f, Pointer extra_data);

int ht_resize(HashTable *ht, size_t new_size);

size_t ht_count(HashTable *ht);

#endif
