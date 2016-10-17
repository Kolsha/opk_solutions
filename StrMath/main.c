#include <stdio.h>
#include <stdlib.h>

#include "mathstr.h"

char *GetTextFromConsole(){
    size_t c = 1;
    int key;
    char *s, *s1;
    s = (char*) malloc((c) * sizeof(char));
    if (s == NULL) {
        return NULL;
    }
    s[c-1] = '\0';
    while ((key = getchar ()) != 10) {
        s[c-1] = key;
        c++;
        s1 = (char*) realloc(s, (c + 1) * sizeof(char));
        if (s1 == NULL){
            break;
        }
        s = s1;
        s[c-1] = '\0';
    }
    s[c] = '\0';
    return s;
}

int main()
{
    char *a,*b;
    printf("Write a and b:\n");
    a = GetTextFromConsole();
    b = GetTextFromConsole();
    //a = "500";
    //b = "50";
    if(a == NULL || b == NULL){
        printf("Memory 404!\n");
        return 1;

    }
    printf("\nMul: %s",nsmul(a, b));
    printf("\nDiv: %s",nsdiv(a, b));
    printf("\nMod: %s",nsmod(a, b));

    printf("\nAdd: %s",nsadd(a, b));
    printf("\nSub: %s",nssub(a, b));

    return 0;
}
