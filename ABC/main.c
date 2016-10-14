#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
    printf("Print A B C:\n");
    int a,b,c;
    double d,x1,x2;
    scanf("%d %d %d",&a,&b,&c);
    if(a==0 && b==0 && c==0){
        printf("Go away");
        return;
    }
    if(a==0){
        printf("%f",(-b/c));
    }
    else{
        d=b*b-4*a*c;
        if(d<0){
            printf("Inf");
            return;
        }
        if(d==0){
            x1=-b/2*a;
            printf("%f",x1);
            return;

        }
        d=sqrt(d);
        x1=(-b+d)/(2*a);
        x2=(-b-d)/(2*a);
        printf("%f %f",x1,x2);
    }
    return 0;
}
