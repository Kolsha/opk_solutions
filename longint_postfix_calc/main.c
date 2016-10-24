#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "calc.h"

int main()
{
    char *test     = "123123123123123123123123123 123 /";
    char *test_res = "1001001001001001001001001";

    assert( strcmp(postfix_calc("8 2 - 2 -"), "4") == 0);

    assert( postfix_calc("113123123123122123123123123 123123123 /") != NULL);



    assert( postfix_calc("2 2 + + +") == NULL);

    assert( postfix_calc("2 2+++") == NULL);

    assert( strcmp(postfix_calc(test), test_res) == 0 );


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
