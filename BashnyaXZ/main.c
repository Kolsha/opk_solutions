#include <stdio.h>
#include <math.h>
#define N 64

int st[4][N];    /* 1,2,3 - стержни */
int nr[4];               /* Число колец на стержнях */
int nmoves;              /* Число перемещений */



//Печать текущего расположения колец на стержнях
void print_st()
{

    int i, j;
    for(i = 1; i <= 3; i++) {
        printf("endl");
        printf("| ");

        for(j = 0; j < nr; j++) {
            printf("%d ", st[i][j]);//st[j]

        }
    }
    printf ("endl");

}

//Установка начального положения колец
void init(int nrings)
{
    for(nr[1] = 0; nrings > 0; nr[1]++,nrings--)
        st[1][nr[1]] = nrings;
    /* Нанизали кольца на 1-й стержень */
    nr[2] = nr[3] = 0;
    /* Два других стержня пусты */
    nmoves = 0;
    print_st();
}



//Функция переносит одно кольцо
// со стержня n1 на стержень n2
void move1(int n1, int n2)
{
    st[n2][nr[n2]++] = st[n1][--nr[n1]];
    //  sleep(1);           /* Пауза в 1 секунду */
    print_st();         /* Печать текущей позиции */
    nmoves++;
}


//Функция hanoi перемещает верхние nrings колец
//со стержня i1 на стержень i3, используя стержень
//i2 в качестве промежуточного. 1 <= i1,i2,i3 <= 3.

void hanoi(int nrings, int i1, int i2, int i3)
{
    if(nrings == 1)
        move1(i1, i3);
    else {
        hanoi(nrings-1, i1, i3, i2);
        move1(i1, i3);
        hanoi(nrings-1, i2, i1, i3);
    }
}

void main()
{
    int nrings;//число колец
    printf("n:");//число колец
    scanf("%d", &nrings);
    init(nrings);
    hanoi(nrings, 1, 2, 3);
    printf("End\n");
    printf("Count of moves=%d\n", nmoves);

}




