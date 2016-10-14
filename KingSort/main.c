#include <stdio.h>

void swap(char *s1,char *s2, size_t N){
    int i;
    char t;
    for(i=0;i<N;i++){
        t=s1[i];
        s1[i]=s2[i];
        s2[i]=t;
    }
}
int IsComp(char *s1,char *s2,size_t N){
    int i;
    for(i=0;i<N;i++){
        if(s1[i]==' '||s2[i]==' ') break;
        if(s1[i]!=s2[i]) return 0;
    }
    if(s1[i]!=s2[i]) return 0;
    return 1;
}

int GetNumber(char *s, size_t N){
    int i,c=-1,r=0;
    char t='\0';
    for(i=0;i<N;i++)if(s[i]==' '||c!=-1){
        switch(s[i]){
        case 'I':
            r++;
            break;
        case 'X':
            r=r+10;
            if(t=='I') r=r-2;
            break;
        case 'V':
            r=r+5;
            if(t=='I') r=r-2;
            break;
        }
        t=s[i];
        c++;
        if(s[i]=='\0') return r;
    }
    return r;
}

void KingSort(char arr[10][21],size_t N){
    int i,j,c1,c2,c,t;
    char temp[21]={'\0'},t1,t2;
    t=1;
    for(i=0;i<N;i++){
        // if(t==0) break;
        t=0;
        for (j=0;j<N-i-1;j++){
            char* tmp1 = arr[j];
            char* tmp2 = arr[j+1];
            for(c=0;c<20;c++){
                t1=tmp1[c];
                t2=tmp2[c];

                if((arr[i][c]==' ' && arr[j][c]!=' ')){
                    swap(tmp1,tmp2,21);
                    break;
                }
                if((tmp1[c]!=' ' && tmp2[c]==' ')){
                    swap(tmp1,tmp2,21);
                    t=1;
                    break;
                }
                if(tmp1[c]>tmp2[c]){
                    t=1;
                    swap(tmp1,tmp2,21);
                    break;
                }
                if(tmp1[c]!=tmp2[c]) break;


            }
        }
    }
    for(i=0;i<N;i++)
        for (j=i;j<N;j++){
            if(IsComp(arr[i],arr[j],21)==1){
                c1=GetNumber(arr[i],21);
                c2=GetNumber(arr[j],21);
                if(c1>c2){
                    swap(arr[i],arr[j],21);
                }
            }
        }


}

int main(int argc, char *argv[])
{
    int i,c=7;
    char arr[10][21]={"Lui XII","Lui V","Lui XI","Lui IX","Lui XXXIV","Lui I","Lui XIX"};

    //printf("Count:\n");
    //scanf("%d",&c);
    // for(i=0;i<c;i++)
    //fgets( arr[i], 20, stdin );
    KingSort(arr,c);
    //KingSort(arr,c);
    for(i=0;i<c;i++)
        printf("%s\n",arr[i]);

    return 0;
}
