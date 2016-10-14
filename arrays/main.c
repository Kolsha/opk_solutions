#include <stdio.h>
#include <math.h>
#include<stdlib.h>
#include <stdbool.h>
#define max_size 10000
int Replace(int *a1,int *a2){
    int b;
    b=*a1;
    *a1=*a2;
    *a2=b;
    return true;
}
int MaxArr(int *a,int n){
    int i=0,m=a[0];
    for (i=0;i<n;i++){
        if(a[i]>m) m=a[i];
    }
    return m;

}

int MinArr(int *a,int n){
    int i=0,m=a[0];
    for (i=0;i<n;i++){
        if(a[i]<m) m=a[i];
    }
    return m;
}
double MidArr(int *a,int n){
    int i=0;
    double m=0;
    for (i=0;i<n;i++){
        m+=a[i];
    }
    m=(double)m/n;

    return m;
}

double RMSArr(int *a,double middle, int n){
    int i=0;
    double m2=0;
    for (i=0;i<(n);i++){
        m2+=pow((double)(middle-a[i]),2);
    }
    m2=(double)m2/n;
    m2=sqrt(m2);
    return m2;
}
int BubbleSort(int *a,int n, bool up){

    bool s=true;
    int i=0,j;
    while (s){
        s = false;
        for(j = 0;j<(n - i - 1);j++)
        {
            if(up && a[j] > a[j + 1])
                s = Replace(&a[j], &a[j + 1]);
            else if(!up && a[j] < a[j + 1])
                s = Replace( &a[j + 1],&a[j]);
        }
        i++;
    }
}
int Last(int *a,int n){
    if(n<=2) return a[0];
    BubbleSort(a,n,true);
    return a[n-2];
}

int main(void)
{   int n,max,min,i,j;
    double middle,m2;
    bool up=false,s=true;
    int *a;
    printf("Size of array:\n");
    scanf("%d",&n);
    a = (int*) malloc((n)*sizeof(int));
    for (i=0;i<(n);i++)
        scanf("%d",&a[i]);
    printf("Action:\n1.Max,Min,Middle,RMS\n2.Sort\n3.Exit\n");
    scanf("%d",&i);
    switch(i){
    case 1:
        min=MinArr(a,n);
        max=MaxArr(a,n);
        middle=MidArr(a,n);
        m2=RMSArr(a,middle,n);
        printf("Max=%d\nMin=%d\nMiddle=%lf\nRMS=%lf",max,min,middle,m2);
        printf("\nLast:%d\n",Last(a,n));
        break;
    case 2:
        printf("\nOrder:\n1.Up\n2.Down\n3.Exit\n");
        scanf("%d",&i);
        if(i>2) return 0;
        up=(i==1);
        /*s=true;
        i=0;
        while (s){
            s = false;
            for(j = 0;j<(n - i - 1);j++)
            {
                if(up && a[j] > a[j + 1])
                    s = Replace(&a[j], &a[j + 1]);
                else if(!up && a[j] < a[j + 1])
                    s = Replace( &a[j + 1],&a[j]);
            }
            i++;
        }*/
        BubbleSort(a,n,up);
        for (i=0;i<(n);i++)
            printf("%d\n",a[i]);
        break;
    default:
        return 0;
    }
    printf("\nNow static:\n");
    int ar[max_size];
    scanf("%d",&n);
    if(n>max_size) return 0;
    for (i=0;i<(n);i++)
        scanf("%d",&ar[i]);
    min=MinArr(a,n);
    max=MaxArr(a,n);
    middle=MidArr(a,n);
    m2=RMSArr(a,middle,n);
    printf("Max=%d\nMin=%d\nMiddle=%lf\nRMS=%lf",max,min,middle,m2);
    printf("\nLast:%d\n",Last(a,n));


    return 0;
}

