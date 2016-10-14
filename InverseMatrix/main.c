#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int Det(int *a, size_t N,int *line,int *col){
    int i=0,j=0,c=0,s=0,run=0,ii,jj;

    for (i=0;i<N;i++){

        if(line[i]!=0){
            ii=i;
            run++;
        }
        if(col[i]!=0){
            jj=i;
            run++;
        }
    }
    if(run<2) {

        return 1;
    }
    if(run==2) {

        return a[ii+N*jj];
    }
    ii=0;
    jj=0;
    for (i=0;i<(N);i++)if(line[i]!=0){
        line[i]=0;
        ii++;
        for (j=0;j<N;j++)
            if(col[j]!=0){
                jj++;
                s=pow(-1,ii+jj);
                s*=a[i+N*j];

                col[j]=0;
                //printf("\nCheck line=%d col=%d\n a=%d",i,j,s);
                s*=Det(a,N,line,col);

                //line[i]=1;
                col[j]=1;
                c+=s;
            }
        line[i]=1;
        break;
    }

    return c;
}

int main()
{   int N,i,j;
    double deter;
    printf("Size Matrix:\n");
    scanf("%d",&N);
    if(N<2) {
        return 0;
    }
    int *m = (int*) malloc((N*N)*sizeof(int));
    int *col = (int*) malloc((N)*sizeof(int));
    int *line = (int*) malloc((N)*sizeof(int));
    if(m==NULL || col==NULL || line==NULL){
        printf("Clean RAM");
        return 0;
    }

    for (i=0;i<N;i++)
    {   line[i]=1;
        col[i]=1;
        printf("Line(%d):",(i+1));
        for (j=0;j<N;j++)
            scanf("%d",&(m[i+j*N]));
    }
    deter=Det(m,N,line,col);
    if(deter==0){
        printf("Det=0");
        return 0;
    }
    for (i=0;i<N;i++)
    {
        for (j=0;j<N;j++){
            line[j]=0;
            col[i]=0;
            printf("%.1f ",(Det(m,N,line,col)/deter));
            line[j]=1;
            col[i]=1;
        }

        printf("\n");
    }
    return 0;
}
