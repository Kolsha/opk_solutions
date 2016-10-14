#include <stdio.h>
#include <math.h>
int main(int argc, char *argv[])
{
    int summa, srok,procent;
    double k=0,s=0;
    printf("Summa srok procent:\n");
    scanf("%d %d %d",&summa, &srok, &procent);
    if(summa==0 || srok==0 || procent ==0){
        printf("Go away");
        return 1;
    }
    s=summa;
    for(int i=0;i<srok;i++){
        k+=summa*procent/12/100.0;
        double k1=s*procent/12/100.0;
        s+=k1;
        printf("%d %lf %lf\n",(i+1), (summa+k),s);
    }
    return 0;
}
