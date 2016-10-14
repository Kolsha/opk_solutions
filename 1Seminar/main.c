#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("Hello, World!\n");
    int i;
    char s=0;
    for(i=1; i<=100;i++){
        s=0;
        if((i%3)==0) {
            s=1;
            printf("Fizz");
        }

        if((i%5)==0) {
            printf("Buzz");
            s=1;
        }
        if(s==0) {
            printf("%d\n",i);
        }
        else {
            printf("\n");
        }

    }
    return 0;
}
