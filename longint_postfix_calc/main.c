#include <stdio.h>

#include "calc.h"

int main()
{
    for(;;){
        printf("Write postfix expr(0 - exit): ");
        char *s = GetTextFrom(stdin);
        if(s == NULL || s[0] == '0'){
            printf("Exit!");
            break;
        }
        printf("Result: %s\n", postfix_calc(s));
        free(s);
    }
    return 0;
}
