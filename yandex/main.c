#include <stdio.h>

int hasOnlyXY(int num, int x, int y)
{
    while (num != 0)
    {
        int tmp = num % 10;
        if(tmp != x && tmp != y)
            return 0;
        num   = num /10;
    }
    return 1;
}




int main()
{
    int N = 0, x = 0, y = 0, count = 0;
    scanf("%d %d %d",&N, &x, &y);


    int i = 0;
    for(i = 1; i <= N; i++){
        count += hasOnlyXY(i, x, y);
    }

    printf("%d", count);
    return 0;
}

/*
#include <stdlib.h>



int checkNumber(char *numb, int min, int max){
    int tmp = strtol(numb, NULL, 10);
    if(tmp >= min && tmp <= max){
        return 1;
    }
    return 0;
}

// memory range
int N = 0, count = 0;
char max = 0, min = 0;


void calcCount(char *number, int pos){
    if(pos < 0)
        return;

    if(min != '0')
        number[pos] = min;
    if(checkNumber(number, 1, N)){
        count++;
    }
    else{
        number[pos] = '0';
        return;
    }
    calcCount(number, pos - 1);

    number[pos] = max;
    if(checkNumber(number, 1, N)){
        count++;
    }
    else{
        number[pos] = '0';
        return;
    }

    calcCount(number, pos - 1);







}



int main()
{

    char x = 0, y = 0;
    //scanf("%d %c %c",&N, &x, &y);

    N = 24;
    x = '2';
    y = '2';

    max = x;
    min = y;

    if(y > x){
        max = y;
        min = x;
    }

    char number[] = "00000000";
    calcCount(number, 7);

    printf("%d", count);
    return 0;
}
*/

/*

*/
