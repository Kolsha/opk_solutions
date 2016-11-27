#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include "str_utils.h"


char *frmt_time(time_t tm){

    if(tm < 0){
        tm = 0;
    }
    char time_str[128];
    struct tm tm_now;

    localtime_r(&tm, &tm_now);

    strftime(time_str, sizeof(time_str), "%M:%S", &tm_now);
    return (char*)strdup(time_str);
}



int mRand(int min, int max){
    return min + rand() % (max - min + 1);
}

char *my_strstr(char *haystack, char *needle){ //horspul

    int lenstr = my_strlen(haystack);
    int lensubstr = my_strlen(needle);

    if (lenstr < 1 || lensubstr < 1 || lensubstr > lenstr){
        return NULL;
    }

    char *table = malloc(256 * sizeof(char));
    if(table == NULL){
        return NULL;
    }

    memset(table, lensubstr, 256 * sizeof(char));
    for(int c = lensubstr - 2; c > 0; c--){
        if(table[(int)needle[c]] == lensubstr)
            table[(int)needle[c]] = lensubstr - c;
    }

    for(int i = 0; i <= (lenstr - lensubstr); i++){
        if (haystack[i] == needle[0]){
            int match = 1;
            for(int c = 0; c < lensubstr; c++)
            {
                match = (haystack[i + c] == needle[c]);
                if(!match){
                    break;
                }
            }

            if(match)
            {
                free(table);
                return &(haystack[i]);
            }
            else{
                int stop = (int)haystack[i + lensubstr - 1];
                i += table[stop];
            }

        }
    }
    free(table);
    return NULL;

}

size_t my_strlen(char *s){

    return (s == NULL) ? 0 : strlen(s);
}

char *my_strcat(char *s1, char *s2){

    size_t len = my_strlen(s1) + my_strlen(s2) + 1;

    if(len == 1){
        return NULL;
    }
    char *res = (char*)malloc(len * sizeof(char));
    if(res == NULL){
        return NULL;
    }
    res[0] = '\0';
    res[len - 1] = '\0';
    if(s1 != NULL)
    {
        strcpy(res, s1);
    }
    if(s2 != NULL)
    {
        strcat(res, s2);
    }
    return res;
}

char *build_request(char *frmt, ...){

    va_list args;

    va_start(args, frmt);

    size_t needed = vsnprintf(NULL, 0, frmt, args) + 1;
    char  *buffer = malloc(needed);
    if(buffer == NULL){
        return NULL;
    }
    int ret;

    ret = vsprintf(buffer, frmt, args);
    va_end(args);

    if(ret < 0){
        free(buffer);
        return NULL;
    }

    return buffer;
}

void _Log_(char *frmt, ...){

    if (frmt == NULL){
        return ;
    }
    va_list args;
    va_start(args, frmt);

    time_t now_t = time(NULL);

    char *now = frmt_time(now_t);
    char *prefix = "[Log] ";
    if(now != NULL){
        prefix = "[Log][%s] ";
        prefix = build_request(prefix, now);
    }

    char *modified = (char*)malloc((strlen(prefix) + strlen(frmt) + 1)*sizeof(char));

    if(modified == NULL){
        return ;
    }
    *modified = '\0';
    strcat(modified, prefix);
    strcat(modified, frmt);
    vprintf(modified, args);
    vprintf("\n", NULL);
    free(modified);
    if(now != NULL){
        free(prefix);
        free(now);
    }
    va_end(args);
}



char get_next_type(char *str, size_t *pos){

    if(str == NULL){
        return 0;
    }

    size_t p = *pos;
    while(str[p] != 0){
        char c = tolower(str[p]);

        if(c == '"' || c == '['
                || c == '{' || c == 't' || c == 'f'
                || c == 'n' || isdigit(c) || c == '-'){

            return c;
        }
        *pos = ++p;
    }

    return 0;
}

int go_to_next(char *str, size_t *pos){
    if(str == NULL){
        return 0;
    }

    size_t p = *pos;
    char cur;
    while((cur = str[p]) != '\0' &&
          (cur = str[p]) != ','  &&
          (cur = str[p]) != '}' ){
        *pos = ++p;
    }

    return (cur != '\0');
}

char *copystr(const char *str, size_t count){

    if(str == NULL || count < 1){
        return NULL;
    }
    char *res = malloc((count + 1) *sizeof(char));
    if(res == NULL){
        return NULL;
    }
    res[count] = 0;
    for(size_t i = 0; i < count; i++){
        if(str[i] == 0){
            break;
        }
        res[i] = str[i];
    }
    return res;
}

char *get_str(char *str, size_t *pos){

    if(str == NULL){
        return NULL;
    }

    size_t p = *pos;
    size_t start = p;
    char prev = 0;
    int count_backslash = 0;
    while(str[p] != 0){
        if(str[p] == '\\'){
            count_backslash++;
        }
        if(str[p] == 'u' && prev == '\\'){
            count_backslash--;
        }
        if(str[p] == '"' &&
                (prev != '\\' || (count_backslash % 2 == 0))){
            *pos = p + 1;
            return copystr(&str[start], p - start);
        }
        prev = str[p];
        *pos = p++;
    }

    return NULL;
}

char *frmt_str(char *one_frmt, char *one_param){

    if(one_frmt == NULL || one_param == NULL){
        return NULL;
    }

    size_t needed =  snprintf(NULL, 0, one_frmt, one_param) + 1;

    char *buffer = malloc(needed);
    if(buffer == NULL){
        return NULL;
    }

    buffer[0] = '\0';

    int sres = snprintf(buffer, needed, one_frmt, one_param);

    if(sres < 0){
        free(buffer);
        return NULL;
    }

    return buffer;
}