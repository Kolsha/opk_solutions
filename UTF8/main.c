#include <stdio.h>

#include "utf16to8.h"

int main(void)
{

    RunTests();
    char *res = utf16s_to_utf8b("\\uD83D\\uDE01 12345647567567567567 \\uD83D\\uDE01 \\u0421");
    if(res != NULL){
        printf("%s\n", res);//
        free(res);
    }
    return 0;
}

