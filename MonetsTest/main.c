#include <stdio.h>
#include <stdlib.h>


int cmp(void *a, void *b){
    int x = *(int*)a;
    int y = *(int*)b;
    if(x > y){
        return 1;
    } else if(x < y){
        return -1;
    }
    return 0;
}

int GetSum(int *arr, size_t count){
    if(arr == NULL || count < 1){
        return -1;
    }
    qsort(arr, count, sizeof(arr[0]), cmp);
    for(size_t n = 1; n < 10; n++){
        size_t sum = 0;
        for(int i = count - 1; i >=0; i--){
            if((sum + arr[i]) <= n){
                sum += arr[i];
            }
            if(sum == n){
                break;
            }
        }
        if(sum != n){
            return n;
        }
    }

    return 0;
}


int main(int argc, char *argv[])
{
    int arr[] = {2,5,9};
    size_t sz = sizeof(arr) / sizeof(arr[0]);

    printf("%d", GetSum(arr, sz));
    return 0;
}
