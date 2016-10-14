#include <stdio.h>
#include <math.h>
#include <stdlib.h>
typedef struct Rect{
    int x1,y1,x2,y2,x3,y3,x4,y4;
} Rect;
int FindIn(Rect r, Rect* a, size_t c){
    int i,b;

    for(i=0; i<c;i++){
        b=1;

        b=b&&((r.x1==a[i].x1&&r.y1==a[i].y1)||(r.x1==a[i].x2&&r.y1==a[i].y2)||
              (r.x1==a[i].x3&&r.y1==a[i].y3)||(r.x1==a[i].x4&&r.y1==a[i].y4));
        //

        b=b&&((r.x1==a[i].x1&&r.y1==a[i].y2)||(r.x2==a[i].x2&&r.y2==a[i].y2)||
              (r.x2==a[i].x3&&r.y2==a[i].y3)||(r.x2==a[i].x4&&r.y2==a[i].y4));
        //
        b=b&&((r.x3==a[i].x1&&r.y3==a[i].y1)||(r.x3==a[i].x2&&r.y3==a[i].y2)||
              (r.x3==a[i].x3&&r.y3==a[i].y3)||(r.x3==a[i].x4&&r.y3==a[i].y4));
        //
        b=b&&((r.x4==a[i].x1&&r.y4==a[i].y1)||(r.x4==a[i].x2&&r.y4==a[i].y2)||
              (r.x4==a[i].x3&&r.y4==a[i].y3)||(r.x4==a[i].x4&&r.y4==a[i].y4));

        //


        //

        if(b==1) return 1;


    }
    return 0;
}

void CountOfRect(int* x, int* y, size_t c){
    int i,j,k,l,h1,h2,w1,w2,d1,d2,cr=0;
    Rect r;
    Rect* a=NULL;
    Rect* ab=NULL;
    if(c<4) return;
    for (i=0;i<c;i++){
        for(j=0;j<c;j++)if(j!=i){

            for(k=0;k<c;k++)if(k!=j){

                for(l=0;l<c;l++)if(l!=k){
                    if((x[i]==x[j]&&y[i]&&y[j])
                            ||
                            (x[i]==x[k]&&y[i]&&y[k])
                            ||
                            (x[i]==x[l]&&y[i]&&y[l])
                            ||
                            (x[l]==x[j]&&y[l]&&y[j])
                            ||
                            (x[k]==x[j]&&y[k]&&y[j])
                            ||
                            (x[k]==x[l]&&y[k]&&y[l])
                            ) continue;
                    w1=sqrt(pow(x[i]-x[j],2)+pow(y[i]-y[j],2));
                    w2=sqrt(pow(x[k]-x[l],2)+pow(y[k]-y[l],2));

                    h1=sqrt(pow(x[i]-x[k],2)+pow(y[i]-y[k],2));
                    h2=sqrt(pow(x[j]-x[l],2)+pow(y[j]-y[l],2));
                    d1=sqrt(pow(x[i]-x[l],2)+pow(y[i]-y[l],2));
                    d2=sqrt(pow(x[k]-x[j],2)+pow(y[k]-y[j],2));
                    r.x1=x[i];
                    r.y1=y[i];
                    r.x2=x[j];
                    r.y2=y[j];
                    r.x3=x[k];
                    r.y3=y[k];
                    r.x4=x[l];
                    r.y4=y[l];

                    //if(w1!=w2 && h1==h2) continue;
                    if(FindIn(r,a,cr)==0&&(((w1*w1+h1*h1)==(d1*d1)))&&(w1!=h1)&&(w2!=h2))
                            {
                        cr++;
                        ab=(Rect*)realloc(a,cr*sizeof(Rect));
                        if(ab==NULL) {
                            printf("fatal");
                            return;
                        }
                        a=ab;
                        a[cr-1]=r;
                        printf("%d - %d\n",a[cr-1].x1,i);

                    }
                }

            }



        }

    }
    printf("%d\n",cr);
    for(i=0;i<cr;i++){
        printf("%d:%d\n",a[i].x1,a[i].y1);
        printf("%d:%d\n",a[i].x2,a[i].y2);
        printf("%d:%d\n",a[i].x3,a[i].y3);
        printf("%d:%d\n",a[i].x4,a[i].y4);
        printf("----\n");
    }

}

int main(int argc, char *argv[])
{
    int c,i;

    printf("Count of points\n");
    scanf("%d",&c);
    if(c<=0) return;
    int* x=(int*)malloc(c*sizeof(int));
    int* y=(int*)malloc(c*sizeof(int));
    if(x==NULL||y==NULL) return 0;
    for(i=0;i<c;i++){
        scanf("%d %d",&x[i],&y[i]);

    }
    CountOfRect(x,y,c);

    return 0;
}
