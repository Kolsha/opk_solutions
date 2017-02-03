#include <stdio.h>
#include <stdlib.h>

#include "count_words.h"
#include "tests.h"

int usage_inf(){

    printf("Usage: <filename>");
    return 1;
}

int main(int argc, char *argv[])
{
    RunTests();
    if(argc != 2){
        return usage_inf();
    }
    FILE *file = fopen(argv[1], "r");
    if(file == NULL){
        return usage_inf();
    }
    char *text = GetTextFrom(file);

    fclose(file);

    CounterResult *res = getstat(text);
    free(text);

    if(res != NULL){
        print_inf(res);
    }else{
        return usage_inf();
    }

    free_res(res);
    return 0;
}
