#include <stdio.h>
#include <math.h>
int main(int argc, char *argv[])
{

    int n,q,c,i,s=2;
    double r=0,a=0,b=0,t=0,l=0;
    printf("Write1 N q\n");
    scanf("%d %d",&n,&q);
    if(n<=2) r=1;
    for(i=1;i<n;i++)
        if((i*i)>n){
            r=i-1;
            break;
        }

    t=pow(10,-q-1);
    while(fabs(r-l)>t||l==0){
        l=r;
        a=(double)1/s;
        b=(double)(s-1)*r;
        b+=(double)n/pow(r,(s-1));
        r=(double)a*b;
    }
    printf("%0*.*f",q,q,r);
    return 0;
}
