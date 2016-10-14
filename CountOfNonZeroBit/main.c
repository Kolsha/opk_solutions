#include <stdio.h>
#include <assert.h>
int CountOfNonZeroBit(const int number){
    int c=0;
    for(int i=1; i<=32;i++){
        if( ((number >> i) & 1) == 1)
        {
            c++;
        }
    }
    return c;
}

int main(int argc, char *argv[])
{
    assert(CountOfNonZeroBit(2)==1);
    assert(CountOfNonZeroBit(4)==1);
    assert(CountOfNonZeroBit(8)==1);
    assert(CountOfNonZeroBit(15)==4);
    assert(CountOfNonZeroBit(0)==0);

    printf("Write number:\n");
    int n;
    scanf("%d",&n);
    //if(n<0) n=-n;
    printf("\nCount of non zero bit: %d \n",CountOfNonZeroBit(n));

    return 0;
}
