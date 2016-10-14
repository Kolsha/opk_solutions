#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(int *a, int *b){
    int c = *b;
    *b = *a;
    *a = c;
}

int mRand(int min, int max){
    return min + rand() % (max - min + 1);
}

long int factorial(int N){
    if (N <= 1) return 1;
    return N * factorial(N-1);
}
void printarr(int *arr, size_t size){
    printf("\n");
    for(size_t i = 0; i < size; i++){
        printf("%d ",arr[i]);
    }
}

char NextSet(int *arr, int n) {
    int j = n - 2;
    while (j != -1 && arr[j] >= arr[j + 1]){
        j--;
    }
    if (j == -1)
    {
        return 0;
    }
    int k = n - 1;
    while (arr[j] >= arr[k]) {
        k--;
    }
    swap(&arr[j], &arr[k]);
    int l = j + 1;
    int r = n - 1;
    while (l<r)
    {
        swap(&arr[l++], &arr[r--]);
    }
    return 1;
}

int *permutations(int N){
    if (N < 1)
        return 0;
    int count = factorial(N);
    int* arr = malloc(sizeof(int) * count * N);
    if (arr == NULL)
        return 0;

    memset(arr,-1,sizeof(int) * count * N);
    for(int i = 0; i < N; i++){
        arr[i] = i;
    }

    while(NextSet(arr,N) == 1){
        int r = mRand(1,count-1);
        while (arr[r*N] != -1){
            r = mRand(1,count-1);
        }
        for(int i = r*N; i < (r*N+N); i++){
            arr[i] = arr[i - r*N];
        }
    }
    for(int i = 0; i < N; i++){
        arr[i] = i;
    }
    return arr;
}

int main()
{
    srand(time(NULL));
    int i = 0;

    printf("Write N:\n");
    scanf("%d", &i);

    int *arr = permutations(i);

    if(arr == 0){
        printf("\nError!\n");
        return 1;
    }

    printarr(arr, factorial(i) * i);


    return 0;
}
