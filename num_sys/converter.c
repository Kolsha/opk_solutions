#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "converter.h"

DoubleNumber *num2dec(char *num, int from){

    if(from < 2 || from > 36 || num == NULL){
        return NULL;
    }

    DoubleNumber *res = (DoubleNumber*) malloc(sizeof(struct tDoubleNumber));
    if(res == NULL){
        return NULL;
    }
    res->part = 0;
    res->whole = 0;
    res->offset_part = 0;

    int digit[256] = {0};
    for(int i = 0; DIGEST[i]; i++){
        digit[(int)DIGEST[i]] = i;
    }

    size_t pos = 0;
    char cur = 0;
    int sum = 0;
    int i_see_punct = 0;

    while((cur = num[pos++]) != '\0'){
        cur = toupper(cur);
        if(!isalnum(cur) && cur != '.'
                && cur != ','){
            free(res);
            return NULL;
        }else if((cur == '.' || cur == ',') &&
                 i_see_punct == 0){
            res->whole = sum;
            sum = 0;
            i_see_punct = 1;
        } else if(!isalnum(cur)) {
            free(res);
            return NULL;
        }

        sum = sum * from + digit[(int)cur];
        if(sum == 0 && i_see_punct){
            res->offset_part++;
        }

    }

    if(i_see_punct){
        res->part = sum;
    }else{
        res->whole = sum;
    }

    if(res->part < 1){
        res->offset_part = 0;
    }

    if(res->offset_part > 0){
        res->offset_part --;
    }




    return res;

}


static char *dec2num_raw(int a, int p) {
    static int i = 0, n;
    char *res = NULL;

    if (a) {
        ++i;
        res = dec2num_raw(a / p, p);
        if(res == NULL){
            return NULL;
        }
        res[n - i--] = DIGEST[a % p];
    }else{
        if(i < 1){
            return strdup("0");
        }
        n = i;
        res = malloc((n + 1) * sizeof(char));
        res[n] = '\0';
    }
    return res;
}

char *dec2num(DoubleNumber *num, int to){

    if(to < 2 || to > 36 ||num == NULL){
        return NULL;
    }
    char *whole = dec2num_raw(num->whole, to);

    if(whole == NULL){
        return NULL;
    }
    char *part = dec2num_raw(num->part, to);

    if(part == NULL){
        free(whole);
        return NULL;
    }

    size_t len_whole = strlen(whole);
    size_t new_size = len_whole + strlen(part) +
            num->offset_part + 2;

    char *res = realloc(whole, new_size);
    if(res == NULL){
        free(whole);
        free(part);
        return NULL;
    }


    res[len_whole] = '.';

    for(int i = 0; i < num->offset_part; i++){
        res[len_whole + i + 1] = '0';
    }
    res[len_whole + num->offset_part + 1] = '\0';
    strcat(res, part);
    free(part);

    return res;
}
