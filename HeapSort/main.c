#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void print_arr(int *arr, size_t size){

    for(size_t i = 0; i < size; i++){
        printf("%d ",arr[i]);
    }
    printf("\n");
}

int mRand(int min, int max){
    return min + rand() % (max - min + 1);
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



int int_cmp(int *x, int *y) {
    int x_value = *x;
    int y_value = *y;

    if (x_value == y_value) {
        return 0;
    } else if (x_value < y_value) {
        return -1;
    } else {
        return 1;
    }
}

void swap_void(void *ptr, size_t size, size_t a, size_t b){
    char tmp;
    for(size_t i = 0; i < size; i++){
        tmp = *((char*)ptr + size * a + i);
        *((char*)ptr + size * a + i) = *((char*)ptr + size * b + i);
        *((char*)ptr + size * b + i) = tmp;
    }
}



void ShiftDown(void *ptr, size_t size,
               int (*cmp)(const void *, const void *), size_t i, size_t j)
{

    size_t left = 2 * i + 1;
    size_t right = left + 1;
    size_t MaxChild = left;
    while (MaxChild < j)
    {
        if (right < j)
        {
            void *elleft = (char*)ptr + size * left;
            void *elright = (char*)ptr + size * right;
            if (cmp(elleft, elright) == -1)
            {
                MaxChild = right;
            }
        }

        void *eli = (char*)ptr + size * i;
        void *elmax = (char*)ptr + size * MaxChild;
        if (cmp(eli,elmax) == -1)
        {

            swap_void(ptr, size, MaxChild, i);
        }
        else
        {
            break;
        }
        i = MaxChild;
        left = 2 * i + 1;
        right = left + 1;
        MaxChild = left;
    }
}

void heapsort(void *, size_t, size_t,
              int (*)(const void *, const void *));

void heapsort(void *ptr, size_t count, size_t size,
              int (*cmp)(const void *, const void *)){
    if(ptr == NULL || count < 1 || size == 0 || cmp == NULL){
        return ;
    }

    for (int i = count / 2 - 1; i >= 0; i--)
    {
        ShiftDown(ptr,  size, cmp, i, count);
    }
    for (int i = count - 1; i > 0; i--)
    {

        swap_void(ptr, size, 0, i);
        ShiftDown(ptr, size, cmp, 0, i);
    }
}

int main()
{
    for(size_t i = 0; i < 100; i++) {
        size_t sz = i;
        int *a = rand_arr(&sz);

        //print_arr(a, sz);

        heapsort(a, sz, sizeof(a[0]), int_cmp);
        assert(checksort(a, sz));
        free(a);
        //print_arr(a,sz);
    }

    printf("Hello World!\n");
    return 0;
}
