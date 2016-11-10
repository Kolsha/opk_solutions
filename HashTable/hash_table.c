#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hash_table.h"


static List *create_list(char *key, Pointer data){

    List *res = (List*)malloc(sizeof(List));
    if(res == NULL){
        return NULL;
    }
    res->data = data;
    res->key = key;
    res->next = NULL;
    return res;
}

static List *find_list(List *head, char *key){
    if(head == NULL || key == NULL){
        return NULL;
    }
    List *nlist = head;
    while(nlist != NULL){
        if(strcmp(nlist->key, key) == 0){
            return nlist;
        }
        nlist = nlist->next;
    }
}

static unsigned jenkins_one_at_a_time_hash(char *key, size_t ht_size) {
    unsigned hash = 0;

    for (; *key; ++key) {
        hash += *key;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash % ht_size;
}

/***********************
    * Public functions  *
    ***********************/

int ht_init(HashTable *ht, size_t size, HashFunction hf, Destructor dtor){
    if(ht == NULL || size < 1){
        return 0;
    }

    List **table = (List**)malloc(sizeof(List*) * size);

    if(table == NULL){
        ht->table = NULL;
        return 0;
    }
    for(size_t i = 0; i < size; i++){
        table[i] = NULL;
    }
    ht->table = table;
    ht->count = 0;
    ht->size = size;
    ht->dtor = dtor;
    ht->hashfunc = (hf == NULL) ? &jenkins_one_at_a_time_hash : hf;

    return 1;
}

Pointer ht_set(HashTable *ht, char *key, Pointer data){

    if(ht == NULL || key == NULL){
        return NULL;
    }
    unsigned index = ht->hashfunc(key, ht->size);
    if (index > ht->size){
        assert(ht_resize(ht, index) == 1);
        return ht_set(ht, key, data);
    }
    List *nlist =find_list(ht->table[index], key);

    if(nlist != NULL){
        (ht->dtor != NULL) ? ht->dtor(nlist->data) : NULL;
        nlist->data = data;
        return data;
    }

    nlist = create_list(key, data);
    assert(nlist != NULL);

    nlist->next = ht->table[index];
    ht->table[index] = nlist;
    ht->count++;
    return data;

}

Pointer ht_get(HashTable *ht, char *key){

    if(ht == NULL || key == NULL){
        return NULL;
    }

    unsigned index = ht->hashfunc(key, ht->size);
    if(index >= ht->size){
        return ;
    }

    List *nlist = find_list(ht->table[index], key);
    if(nlist != NULL){
        return nlist->data;
    }

    return NULL;
}

int ht_has(HashTable *ht, char *key){

    if(ht == NULL || key == NULL){
        return 0;
    }

    unsigned index = ht->hashfunc(key, ht->size);
    if(index >= ht->size){
        return 0;
    }

    return (ht->table[index] != NULL);
}

int ht_has_strict(HashTable *ht, char *key){

    return (ht_get(ht, key) != NULL);
}

int ht_resize(HashTable *ht, size_t new_size){
    if(ht == NULL || new_size < 1 ){
        return 0;
    }

    List **htable = ht->table;
    size_t hsize = ht->size;

    List **table = (List**)malloc(sizeof(List*) * new_size);

    if(table == NULL){
        return 0;
    }

    for(size_t i = 0; i < new_size; i++){
        table[i] = NULL;
    }
    ht->count = 0;
    ht->size = new_size;
    ht->table = table;

    if(htable == NULL){
        return 1;
    }

    for(size_t i = 0; i < hsize; i++){
        if(htable[i] != NULL)
        {
            assert(ht_set(ht, htable[i]->key, htable[i]->data) != NULL);
        }
    }

    return 1;

}

size_t ht_count(HashTable *ht){

    return (ht != NULL) ? ht->count : 0;
}

void ht_traverse(HashTable *ht, int (*f)(char *key, Pointer data)){
    if(ht == NULL || ht->count < 1 || f == NULL){
        return ;
    }

    for(size_t i = 0;  i < ht->size; i++){
        if(ht->table[i] == NULL){
            continue;
        }

        List *nlist = ht->table[i];
        while(nlist != NULL){
            if(!f(nlist->key, nlist->data)){
                return ;
            }
            nlist = nlist->next;
        }
    }
}

void ht_destroy(HashTable *ht){

    if(ht == NULL){
        return ;
    }
    for(size_t i = 0;  i < ht->size; i++){
        if(ht->table[i] == NULL){
            continue;
        }
        List *nlist = ht->table[i] , *tmp;
        while(nlist != NULL){
            (ht->dtor != NULL) ? ht->dtor(nlist->data) : NULL;
            tmp = nlist->next;
            free(nlist);
            nlist = tmp;
        }
    }
    free(ht->table);
    free(ht);

}

void ht_delete(HashTable *ht, char *key){

    if(ht == NULL || key == NULL){
        return ;
    }

    unsigned index = ht->hashfunc(key, ht->size);
    if(index >= ht->size){
        return ;
    }

    List *prev = NULL, *cur =  ht->table[index], *next = cur->next;
    while(cur != NULL){
        next = cur->next;
        if(strcmp(cur->key, key) == 0){
            (ht->dtor != NULL) ? ht->dtor(cur->data) : NULL;
            free(cur);
            if(prev == NULL){
                ht->table[index] = next;
            }else{
                prev->next = next;
            }
            if(ht->count > 0)
            {
                ht->count--;
            }
            return ;
        }
        prev = cur;
        cur = cur->next;

    }

}
