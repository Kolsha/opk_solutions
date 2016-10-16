#include <stdio.h>
#include <stdlib.h>

#include "mathstr.h"

unsigned char *GetTextFromConsole(){
    size_t c=1;
    int key;
    unsigned char *s,*s1;
    s = (char*) malloc((c)*sizeof(char));
    if (s==NULL) return NULL;
    s[c-1]='\0';
    while ((key = getchar ()) != 10) {
        s[c-1]=key;
        c++;
        s1 = (char*)realloc(s, (c+1)*sizeof(char));
        if (s1==NULL) break;
        s=s1;
        s[c-1]='\0';
    }
    s[c]='\0';
    return s;
}

int main()
{
    char *a,*b;
    printf("Write a and b:\n");
    a = GetTextFromConsole();
    b = GetTextFromConsole();
    if(a == NULL || b == NULL){
        printf("Memory 404!\n");
        return 1;

    }
    printf("\n%s",nsdiv(a, b));

    return 0;
}
