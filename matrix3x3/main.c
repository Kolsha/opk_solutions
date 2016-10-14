#include <stdlib.h>
#include <stdio.h>

double Deter(int a[4][4]){
    double det;
    det=a[1][1]*a[2][2]*a[3][3]+a[2][1]*a[3][2]*a[1][3]+a[1][2]*a[2][3]*a[3][1]-a[1][3]*a[2][2]*a[3][1]-a[3][2]*a[2][3]*a[1][1]-a[2][1]*a[1][2]*a[3][3];
    return det;
}
void InverseMatrix(int a[4][4], double b[4][4], double det){
    b[1][1]=(double)(a[2][2]*a[3][3]-a[3][2]*a[2][3])/(1.0*det);
    b[1][2]=(double)(a[1][3]*a[3][1]-a[3][3]*a[1][2])/(1.0*det);
    b[1][3]=(double)(a[1][2]*a[2][3]-a[1][3]*a[2][2])/(1.0*det);
    b[2][1]=(double)(a[2][3]*a[3][1]-a[2][1]*a[3][3])/(1.0*det);
    b[2][2]=(double)(a[1][1]*a[3][3]-a[1][3]*a[3][1])/(1.0*det);
    b[2][3]=(double)(a[1][3]*a[3][1]-a[1][1]*a[2][3])/(1.0*det);
    b[3][1]=(double)(a[2][1]*a[3][2]-a[2][2]*a[3][1])/(1.0*det);
    b[3][2]=(double)(a[1][2]*a[3][1]-a[3][2]*a[1][1])/(1.0*det);
    b[3][3]=(double)(a[1][1]*a[2][2]-a[1][2]*a[2][1])/(1.0*det);
}

int main()
{
    int i,j,a[4][4];
    double b[4][4],det;
    printf("Write Matrix:\n");
    for(i=1;i<=3;i++)
        for (j=1;j<=3;j++)
            scanf("%d", &a[i][j]);
    det=Deter(a);
    if (det!=0)
    {

        printf("determinant=%lf\n", det);
        InverseMatrix(a,b,det);
        for (i=1;i<=3;i++)
        {
            for (j=1;j<=3;j++)
            {
                printf("%f ",b[i][j]);
            }
            printf("\n");
        }
    }
    else
        printf("determinant=0\n");

    return 0;

}
