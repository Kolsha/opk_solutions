#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#define GETBIT(x,pos)   ( ((x) & ( 0x1 << (pos) )) !=0 )


void print_arr(int *arr, size_t size){

    for(size_t i = 0; i < size; i++){
        printf("%d ",arr[i]);
    }
    printf("\n");
}

int mRand(int min, int max){
    return min + rand() % (max - min + 1);
}

void swap(int *a, int *b){
    int c = *b;
    *b = *a;
    *a = c;
}
void fill_arr(int *arr, size_t size){

    for(size_t i = 0; i < size; i++){
        arr[i] = rand();
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

void coctail_sort(int *a, size_t size){

    int left = 0;
    int right = size - 1;
    while (left <= right)
    {
        for (int i = left; i < right; i++)
        {
            if(a[i] > a[i+1])
            {
                swap(&a[i],&a[i+1]);
            }
        }
        right--;
        for (int i = right; i > left ; i--)
        {
            if(a[i] < a[i-1])
            {
                swap(&a[i],&a[i-1]);
            }
        }
        left++;
    }
}
void select_sort(int *a, size_t size){

    for (size_t i = 0; i < size - 1; i++) {
        size_t least = i;
        for (size_t j = i + 1; j < size; j++) {
            if(a[j] < a[least]) {
                least = j;
            }
        }
        swap(&a[i], &a[least]);
    }
}

void insert_sort(int *array, size_t size){


    for (size_t i = 1; i < size; i++)
    {
        int temp = array[i];
        for (int j = i - 1; j >= 0; j--)
        {
            if (array[j] < temp)
                break;

            array[j + 1] = array[j];
            array[j] = temp;
        }
    }
}

void bubble_sort(int *a, size_t size){

    for (size_t i = 0; i < size-1; i++) {
        char swapped = 0;
        for (size_t j = 0; j < size-i-1; j++) {
            if (a[j] > a[j+1]) {
                swap(&a[j+1],&a[j]);
                swapped = 1;
            }
        }
        if(swapped == 0)
            break;
    }
}
void quicksort(int *a, size_t start, size_t end){
    if(a == NULL || end < 1 || start > end){
        return ;
    }
    int x = a[start + (end - start)/2];
    size_t l = (size_t) start;
    size_t r = (size_t) end;
    while (l <= r){
        while(a[l] < x && l <= end){
            l++;
        }
        while(a[r] > x && r <= end){
            r--;
        }
        if(l <= r){
            //if(a[l] > a[r]){
                swap(&a[l],&a[r]);
            //}
            l++;
            if(r > 0)
            {
                r--;
            }
        }


    }
    if (l < end)
        quicksort(a, l, end);
    if (start < r)
        quicksort(a, start, r);


}


void radixsort(int *a, size_t size){
    if(a == NULL){
        return ;
    }
    int a0c = sizeof(a[0]) * size;
    int* a0 = malloc(a0c);
    int* a1 = malloc(a0c);
    int a1c = 0;
    a0c = 0;
    int pos = 0;
    int maxpos = sizeof(a[0]) * 8 - 1;
    while(pos <= maxpos){
        for(size_t i = 0; i < size; i++){
            int bit = GETBIT(a[i], pos);
            if (GETBIT(a[i],maxpos) == 1)
                bit = !bit;
            if(bit == 1){
                a1[a1c] = a[i];
                a1c++;
            }else{
                a0[a0c] = a[i];
                a0c++;
            }
        }
        for(int i = 0; i < a0c; i++){
            a[i] = a0[i];
        }
        for(int i = 0; i < a1c; i++){
            a[a0c + i] = a1[i];
        }

        a1c = 0;
        a0c = 0;
        pos++;
    }

    for(size_t i = 0; i < size; i++){
        if(a[i] >= 0){
            a1[a1c] = a[i];
            a1c++;
        }else{
            a0[a0c] = a[i];
            a0c++;
        }
    }
    for(int i = 0; i < a0c; i++){
        a[i] = a0[a0c - i - 1];
    }
    for(int i = 0; i < a1c; i++){
        a[a0c + i] = a1[i];
    }
    free(a0);
    free(a1);
}

int checksort(int *a, size_t size){
    if(a == NULL){
        return -1;
    }
    for(size_t i = 0; i < size - 1; i++){
        if(a[i] > a[i+1]){
            return 0;
        }
    }
    return 1;
}

int main(){

    srand(time(NULL));

    for(int i = 0; i < 5; i++){
        clock_t time;
        size_t sz = 1000000;
        printf("Test#%d - Size: %d\n", (i+1), (int)sz);

        int *arr_single = rand_arr(&sz);
        int *arr = malloc(sz * sizeof(arr_single[0]));

        memcpy(arr, arr_single, sz*sizeof(arr_single[0]));
        time = clock();
        radixsort(arr, sz);
        time = clock() - time;

        assert(checksort(arr,sz) == 1);
        printf("Radix: %f\n", (double)time/CLOCKS_PER_SEC);


        memcpy(arr,arr_single,sz*sizeof(arr_single[0]));
        time = clock();
        quicksort(arr, 0, sz-1);
        time = clock() - time;
        assert(checksort(arr,sz) == 1);
        printf("Quick: %f\n", (double)time/CLOCKS_PER_SEC);


        memcpy(arr,arr_single,sz*sizeof(arr_single[0]));
        time = clock();
        insert_sort(arr,sz);
        time = clock() - time;

        assert(checksort(arr,sz) == 1);
        printf("Insert: %f\n", (double)time/CLOCKS_PER_SEC);
        free(arr);
        free(arr_single);

    }
    return 0;
}
