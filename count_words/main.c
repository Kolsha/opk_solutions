#include <stdio.h>
#include "count_words.h"

int usage_inf(){

    printf("Usage: <filename>");
    return 1;
}

int main(int argc, char *argv[])
{
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

    free(res);
    return 0;
}