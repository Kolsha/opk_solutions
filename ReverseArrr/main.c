#include <stdio.h>

void reverse(int arr[], size_t count)
{
    if(arr==NULL) return;
    int i;

    for(i=0; i <= ((count-1)/2); i++){
        int a = arr[i];
        arr[i] = arr[count-i-1];
        arr[count-i-1] = a;
    }
}
int main(int argc, char *argv[])
{
    int a[] = {1,2, 3, 4};
    int arr_sz = sizeof(a)/sizeof(int);
    printf("%d\n",arr_sz);
    reverse(a,arr_sz);
    int i;
    for(i=0; i<arr_sz; i++)
        printf("%d\n",a[i]);
    return 0;
}
