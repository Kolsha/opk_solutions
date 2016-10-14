#include <stdio.h>
#include<math.h>

double f_x(double x){
    double f_x=(double)((x));

    return f_x;
}
int main(void)
{
    int a,b,n;
    double s,h,x;
    printf("Lim a,b\n");
    scanf("%d %d",&a,&b);
    printf("Otr n\n");
    scanf("%d",&n);
    if(n==0 ||(a==0&&b==0)) {
        printf("hacker");
        return 0;
    }
    h=(double)(a-b)/n;
    s=(f_x(a)+f_x(b))/2.0;
    x=b;
    while(x<a){
        x+=h;
        s+=f_x(x);
    }
    s=s-f_x(x);
    s*=h;
    printf("S=%lf",s);
    return 0;
}


