#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "avltree.h"

int mRand(int min, int max){
    return min + rand() % (max - min + 1);
}

void fill_arr(int *arr, size_t size){

    for(size_t i = 0; i < size; i++){
        arr[i] = mRand(1, size);
        if(arr[i] % mRand(1,3) == 0)
            arr[i] = -arr[i];
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

void foreach_func(Pointer data, Pointer extra_data){

    assert(extra_data == for_foreach);
    int tmp = *(int*)data;
    printf("\n%s - %d", (char*)extra_data, tmp);

}

int main()
{
    srand(time(NULL));
    AVLTree *tree = avl_create(cmp_int);

    assert(tree != NULL);
    assert(avl_size(tree) == 0);

    size_t sz = 10;
    int *arr = rand_arr(&sz);
    assert(arr != NULL);

    for(size_t i = 0; i < sz; i++){
        assert(avl_insert(tree, &arr[i]) == &arr[i]);
    }

    for(size_t i = 0; i < sz; i++){
        assert(cmp_int(avl_find(tree, &arr[i]), &arr[i]) == 0);
    }
    avl_foreach(tree, foreach_func, for_foreach);


    printf("\nAll test passed!");
    print_tree(tree->root);
    free(arr);
    avl_destroy(tree);

    return 0;
}
