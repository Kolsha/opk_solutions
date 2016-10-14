#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define X0 3
typedef struct
{
    int   a;
    int c;
    int length;
    int X;
} Comb;


int Rnd(int x, int a, int c, int m){
    return (a*x+c)%m;
}
int InArr(int *arr, int a,int c){
    int i;
    for (i=0;i<c;i++)
        if(arr[i]==a) return 1;
    return 0;
}

int Length(int x, int a, int c, int m){
    int *arr = (int*) malloc((m*m)*sizeof(int));
    memset(arr,-1,m*m);
    int count=0,i=0,z=Rnd(x,a,c,m);
    while(InArr(arr,z,count)==0){
        arr[count]=z;
        z=Rnd(z,a,c,m);
        //printf("%d\n",z);
        count++;
    }
    free(arr);
    return count;
}

void InitComb(Comb* arr, size_t N){
    int i;
    for (i=0; i<N;i++)
    {
        arr[i].a=-1;
        arr[i].c=-1;
        arr[i].X=-1;
        arr[i].length=-1;

    }
}

void MaxLengthByM(int m){
    int ai,ci,xi,c=0,Ml=-1,Mi=-1;
    Comb *arr = (Comb*) malloc((m*m)*sizeof(Comb));
    InitComb(arr,m*m);

    for(ai=0;ai<(m-1);ai++){
        for(ci=0;ci<(m-1);ci++){
            arr[c].a=ai;
            arr[c].c=ci;

            for(xi=0;xi<(m-1);xi++){
                arr[c].length=Length(m,ai,ci,m);
                if(arr[c].length>Ml){
                    arr[c].X=xi;
                    Mi=c;
                    Ml=arr[c].length;
                }
            }
            c++;
        }
    }
    printf("Max length: %d\nA: %d\nC: %d\nX: %d\nM: %d",
           arr[Mi].length,arr[Mi].a,arr[Mi].c,arr[Mi].X,m);

}

int main(int argc, char *argv[])
{   MaxLengthByM(16);
    //printf("%d\n",Length(X0,3,4,77));
    return 0;
}
