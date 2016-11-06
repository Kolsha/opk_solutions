#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "tests.h"
#include "rbtree.h"

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

void print_tree(RBTreeNode *node, int level){

    if(node == NULL){
        printf("L(%d) Empty\n", level);
        return ;
    }
    int tmp = *(int*)node->data;
    printf("H(%d) { %d }\n", level, tmp);

    if(node->left != NULL)
    {
        printf("Left:");
        print_tree(node->left, level + 1);
    }
    if(node->right != NULL){
        printf("Right:");
        print_tree(node->right, level + 1);
    }
}

void RunTests(){

    srand(time(NULL));
    {
        RBTree *tree = rb_create(cmp_int);

        assert(tree != NULL);
        assert(rb_size(tree) == 0);

        size_t sz = 10;

        int arr[] = {1, 2, 3, 4, 5, 0, 7, 6};
        sz = sizeof(arr) / sizeof(arr[0]);

        for(size_t i = 0; i < sz; i++){
            assert(rb_insert(tree, &arr[i]) == &arr[i]);
            assert(rb_check(tree) == 1);
        }

        for(size_t i = 0; i < sz; i++){
            assert(cmp_int(rb_find(tree, &arr[i]), &arr[i]) == 0);
        }
        rb_destroy(tree);
    }
    {
        RBTree *tree = rb_create(cmp_int);

        size_t sz = 1000;
        int *arr_d = rand_arr(&sz);
        assert(arr_d != NULL);

        for(size_t i = 0; i < sz; i++){
            assert(cmp_int(rb_insert(tree, &arr_d[i]), &arr_d[i]) == 0);
            assert(rb_check(tree) == 1);
        }

        rb_foreach(tree, foreach_func, for_foreach);


        free(arr_d);
        rb_destroy(tree);
    }
    printf("\nAll test passed!");

}
