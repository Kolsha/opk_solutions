#include <stdio.h>
#include <math.h>
#include <assert.h>

double *find_el(double *arr, size_t sz, int cmp(double, double)){
    if(cmp == NULL){
        return NULL;
    }
    if(arr == NULL || sz < 1){
        return NULL;
    }
    size_t el = 0;
    for(size_t i = 0; i < sz; i++){
        if(cmp(arr[i], arr[el]) == 1){
            el = i;
        }
    }
    return &arr[el];
}

int cmp_max(double a, double b){
    if(a > b){
        return 1;
    }else if(a < b){
        return -1;
    }
    return 0;
}

int cmp_min(double a, double b){
    if(a < b){
        return 1;
    }else if(a > b){
        return -1;
    }

    return 0;
}

void FindTests(){
    double arr[] = {10, 11, 13, 0, -1, 33, 12};
    size_t arr_sz = sizeof(arr) / sizeof(double);
    double el_max = *find_el(arr, arr_sz, &cmp_max);
    double el_min = *find_el(arr, arr_sz, &cmp_min);
    assert(el_max == 33);
    assert(el_min == -1);
}
double *average_el(double *arr, size_t sz, double approximation){
    if(arr == NULL || sz == 0){
        return NULL;
    }

    approximation = fabs(approximation);

    double el_max = *find_el(arr, sz, &cmp_max);
    double el_min = *find_el(arr, sz, &cmp_min);
    double average = fabs((el_max + el_min) / 2);


    for(size_t i = 0; i < sz; i++){
        double tmp = fabs(average - fabs(arr[i]));
        if(tmp < approximation){
            return &arr[i];
        }
    }

    return &arr[0];
}
void AverageTests(){
    double arr[] = {-1488, 1945, 228, 666, -777, 42, 12, 1234, 2345, -42, 22, 34, 100, 300, 1337};
    size_t arr_sz = sizeof(arr) / sizeof(double);
    double average_all = *average_el(arr, arr_sz, 150);
    assert(average_all == 300);
    double average_5_10 = *average_el(&arr[5], 5, 150);
    assert(average_5_10 == 1234);
}


int main(void)
{
    printf("Hello World!\n");
    FindTests();
    AverageTests();
    return 0;
}
