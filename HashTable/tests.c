#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "tests.h"
#include "hash_table.h"

int mRand(int min, int max){
    return min + rand() % (max - min + 1);
}

void fill_arr(int *arr, size_t size){

    for(size_t i = 0; i < size; i++){
        arr[i] = i;
    }
}

int *rand_arr(size_t *sz){
    const int MaxSize = 50;
    if(sz == NULL){
        return NULL;
    }

    size_t size = 0;

    if(*sz > 0){
        size = *sz;
    }
    else{
        size = mRand(2, MaxSize);
    }
    int *arr = malloc(size * sizeof(int));
    if (arr == NULL){
        return NULL;
    }
    *sz = size;
    fill_arr(arr, size);
    return arr;
}

int cmp_int(Pointer a, Pointer b){

    int x = *(int*)a;
    int y = *(int*)b;
    if(x > y){
        return 1;
    }else if(x < y){
        return -1;
    }
    return 0;
}

char *for_foreach = "Test";

int foreach_func(Pointer data, Pointer extra_data){

    assert(extra_data == for_foreach);
    int tmp = *(int*)data;
    printf("\n%s - %d", (char*)extra_data, tmp);
    return 1;
}

void RunTests(){
    {
        HashTable ht;

        assert(ht_init(&ht, 100, NULL, NULL) == 1);


        assert(ht_set(&ht, "Vasya", (Pointer)"333-45-67") != NULL);
        assert(ht_set(&ht, "Police", (Pointer)"02") != NULL);

        assert(ht_count(&ht) == 2);

        assert(ht_resize(&ht, 50));

        assert(ht_count(&ht) == 2);

        assert(!strcmp(ht_get(&ht, "Vasya"), "333-45-67"));

        assert(ht_has(&ht, "Police"));

        assert(ht_set(&ht, "Police1", (Pointer)"022") != NULL);
        assert(ht_set(&ht, "Police2", (Pointer)"023") != NULL);

        assert(ht_count(&ht) == 4);

        ht_delete(&ht, "Police1");

        assert(ht_count(&ht) == 3);

        ht_destroy(&ht);



    }

    printf("\nAll test passed!");

}
