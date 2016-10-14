#include <stdio.h>
#include <stdlib.h>


double CalcPi(const long long int N){

    long long int i,GoodTry=0;
    for(i=0; i<N; i++){
        double x=rand()%200/100.0 - 1;
        double y=rand()%200/100.0 - 1;
        if((x * x + y * y) <= 1) {
            GoodTry++;
        }
    }
    return 4.0*GoodTry/(double)N;

}

int main()
{
    srand ( time(NULL) );

    printf("Debug info begin:\n");

    long   int c,d=10;
    for(c=1;c<100000000000LL;c++)
    {
        if(c%d==0){
            printf("N=%d ~Pi=%lf\n",c, CalcPi(c));
            d=d*10;
        }
    }
    printf("Debug info end;\n");

    int N;
    printf("Count of try:\n");
    scanf("%d",&N);

    if(N<=0){
        printf("Go away");
        return 1;
    }

    double pi = CalcPi(N);
    printf("~Pi=%lf",pi);

    return 0;
}
