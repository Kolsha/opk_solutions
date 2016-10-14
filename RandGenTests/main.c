#include <stdio.h>
#include <stdlib.h>
int main()
{
    srand( time(NULL) );
    int c = (rand()%100 + 1) * 100000;
    int numbers[100] = {0};
    for(int i = 0; i < c; i++){
        numbers[rand()%100]++;
    }
    for(int i = 0; i < 100; i++){
        printf("%d - %lf\n", i, (numbers[i]/(double)c) );
    }
    return 0;
}
