#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

int check(char *haystack, char *needle, char* (*find)(char *, char*)){
    char *res = find(haystack, needle);
    if (res == NULL){
        return -1;
    }
    return (res - haystack);
}

char *my_strstr_fird(char *haystack, char *needle){ //horspul

    int lenstr = strlen(haystack);
    int lensubstr = strlen(needle);

    if (lenstr < 1 || lensubstr < 1 || lensubstr > lenstr){
        return NULL;
    }

    int *table = malloc(256 * sizeof(int));
    if(table == NULL){
        return NULL;
    }
    memset(table,lensubstr ,256 * sizeof(int));
    for(int c = lensubstr - 2; c > 0; c--){
        if(table[(int)needle[c]] == lensubstr)
            table[(int)needle[c]] = lensubstr - c;
    }

    for(int i = 0; i <= lenstr-lensubstr; i++){
        if (haystack[i] == needle[0]){
            char match = 1;
            for(int c=0; c < lensubstr; c++)
            {
                match = (haystack[i+c] == needle[c]);
                if(!match) break;
            }
            if(match)
            {
                return &(haystack[i]);
            }
            else{
                char stop = haystack[i+lensubstr-1];
                i+= table[(int)stop];
            }

        }
    }
    return NULL;

}

char *my_strstr_second(char *haystack, char *needle){ //only bair

    int lenstr = strlen(haystack);
    int lensubstr = strlen(needle);
    if (lenstr < 1 || lensubstr < 1 || lensubstr > lenstr){
        return NULL;
    }
    int *table = malloc(256 * sizeof(int));
    if(table == NULL){
        return NULL;
    }
    memset(table, 0, 256 * sizeof(int));
    for(int c = lensubstr - 2; c >= 0; c--){
        if(table[(int)needle[c]] == 0)
            table[(int)needle[c]] = lensubstr - c;
    }
    int offset = 0;
    while((offset+lensubstr) <= lenstr){
        char stop = 0;
        int i;
        for(i = offset + lensubstr - 1; i > 0; i--){
            if(needle[i-offset] == 0){
                break;
            }
            if(haystack[i] != needle[i-offset]){
                stop = haystack[i];
                break;
            }
        }
        if(stop == 0){
            return &(haystack[offset]);
        }
        if (table[(int)stop] == 0){
            offset += i + 1;
        }
        else{
            offset += table[(int)stop];
        }
    }
    /*
            char match = 1;
            for(int c=0; c < lensubstr; c++)
            {
                match = (haystack[i+c] == needle[c]);
                if(!match) break;
            }
            if(match)
            {
                return &(haystack[i]);
            }
            else{
                char stop = haystack[i+lensubstr-1];
                i+= table[(int)stop];
            }

      */
    return NULL;

}

char * my_strstr(char *haystack, char *needle){ //Кнута — Морриса — Пратта
    size_t lenstr = strlen(haystack);
    size_t lensubstr = strlen(needle);
    if (haystack == NULL || needle == NULL || (lenstr - lensubstr) < 1){
        return NULL;
    }
    int *pref = malloc(lensubstr * sizeof(int));

    if(pref == NULL){
        return NULL;
    }

    pref[0] = 0;

    size_t k = 0;
    for (size_t i = 1; i < lensubstr; ++i) {
        while ((k > 0) && (needle[k] != needle[i]))
            k = pref[k - 1];
        if (needle[k] == needle[i])
            ++k;
        pref[i] = k;
    }

    int index = -1;

    k = 0;
    for (size_t i = 0; i < lenstr; i++){
        while (k > 0 && haystack[i] != needle[k])
        {
            k = pref[k-1];
        }
        if (needle[k] == haystack[i])
        {
            k = k + 1;
        }
        if (k == lensubstr){
            index = i - lensubstr + 1;
            break;
        }
    }
    free(pref);
    if(index < 0){
        return NULL;
    }
    return &haystack[index];

}

int main()
{
    clock_t time;
    time = clock();
    assert(3 == check("abcdef", "def", my_strstr));
    assert(-1 == check("abcdef", "xdef", my_strstr));
    time = clock() - time;
    printf("%f", (double)time/CLOCKS_PER_SEC);


    return 0;
}
