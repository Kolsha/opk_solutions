#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    int N, K,c,i;
    printf("N K:\n");
    scanf("%d %d",&N,&K);
    if(N==0 || K==0){
        printf("Go away");
        return;
    }
    int *arr = malloc(N*sizeof(int));
    if(arr==NULL){
        printf("Not today");
        return;
    }
    c=0;
    for(i=0; i<N;i++){

        arr[i]=1;
    }

    i=0;
    int si=1;
    while((N-c)>1){
        if(arr[i]!=0){
            if(si==K)
            {
                arr[i]=0;
                si = 0;
                c++;
            }
            si++;

        }
        i++;
        if(i>=N)
            i=0;
    }

    for(i=0; i<N;i++)
    {
        if(arr[i]!=0){

            printf("Winner: %d",i+1);
            return;
        }
    }
    free(arr);


    return 0;
}
