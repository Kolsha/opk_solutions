#include "count_words.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "hash_table.h"

char *GetTextFrom(FILE *input){

    size_t inc = 16;
    size_t str_len = 0;
    size_t buf_size = inc;
    char *buf = malloc(buf_size * sizeof(char));
    if (buf == NULL) {
        return NULL;
    }

    if (input == NULL) {
        return NULL;
    }

    for (;;) {
        // read next part
        if (fgets(buf + str_len, buf_size - str_len, input) == NULL || feof(input)) {
            if (feof(input) && str_len > 0) {
                // read all given input
                break;
            } else {
                // no input at all or some error
                free(buf);
                return NULL;
            }
        }

        size_t added_len = strlen(buf + str_len);
        if (added_len == 0) {
            // there is some input but its length is zero
            // (e.g. "\0foo\n")
            // treat it as error
            free(buf);
            return NULL;
        }
        str_len += added_len;
        assert(str_len + 1 <= buf_size);
        assert(buf[str_len] == '\0');

        // otherwise resize buffer for reading next part
        inc++;
        size_t new_buf_size = buf_size + inc;
        if (new_buf_size <= buf_size) {
            // integer overflow, we will be out of memory
            free(buf);
            return NULL;
        }
        char *new_buf = realloc(buf, new_buf_size);
        if (new_buf == NULL) {
            free(buf);
            return NULL;
        }
        buf_size = new_buf_size;
        buf = new_buf;
    }

    assert(str_len > 0);

    // trim trailing '\n' if any
    if (buf[str_len - 1] == '\n') {
        buf[str_len - 1] = '\0';
    }

    // cut buffer to actual length
    char *result = realloc(buf, str_len + 1);
    if (result == NULL) {
        free(buf);
        return NULL;
    }
    return result;
}

static int IsAlphabetic(char c){

    if(c == 241||c == 240) return 1; // ||c==34
    if(c >= 128 && c <= 175) return 1;
    if(c >= 224 && c <= 239) return 1;
    if(isalnum(c)) return 1;
    return 0;
}

static char *getword(char *str, size_t *pos){

    if(str == NULL){
        return NULL;
    }
    char arr[50] = {0};
    size_t i = 0;
    size_t p = *pos - 1;
    while(str[*pos] != 0 && isalpha(str[p])){
        arr[i++] = tolower(str[p++]);
    }
    *pos = p;
    return strdup(arr);
}

CounterResult *getstat(char *str){

    if(str == NULL){
        return NULL;
    }

    CounterResult *res = (CounterResult*) malloc(sizeof(CounterResult));
    if(res == NULL){
        return NULL;
    }
    ht_init(&(res->hWords), 100, NULL, NULL);

    res->lines = 1;
    res->symbols = 0;
    res->words = 0;
    res->punctuation = 0;
    res->whitespace = 0;
    res->numbers = 0;

    char p = '\0', c = '\0';

    int prev_space = 1;

    while((c = str[res->symbols++]) != '\0')
    {
        if(isalpha(c)){
            char *word = getword(str,&(res->symbols));
            Pointer tmp = ht_get(&(res->hWords), word);
            size_t t = 0;
            if(tmp != NULL){
                t = (size_t)tmp;
            }
            t++;
            assert(ht_set(&(res->hWords), word, (Pointer)t) != NULL);

            continue;
        }
        if(c == '\n'){
            res->lines++;
        }

        if(isdigit(c)){
            res->numbers++;
        }

        if(isspace(c)){
            res->whitespace++;
            if(prev_space){
                continue;
            }
            res->words++;
        }

        if(ispunct(c)){
            res->punctuation++;
        }

        p = c;
        prev_space = isspace(c);
    }
    if(IsAlphabetic(p)){
        res->words++;
    }
    return res;
}

static int print_words(char *key, Pointer data){

    printf("%s - %d\n", key, (size_t)data);
}

void print_inf(CounterResult *res){

    if(res == NULL){
        printf("\nBad result!");
        return ;
    }

    printf("\nLines: %d", res->lines);
    printf("\nWords: %d", res->words);
    printf("\nWhitespace: %d", res->whitespace);
    printf("\nPunctuation: %d", res->punctuation);
    printf("\nSymbols: %d", res->symbols);
    printf("\nNumbers: %d", res->numbers);
    printf("\nWords stat: \n\n", res->numbers);

    ht_traverse(&(res->hWords), &print_words);
}

void free_res(CounterResult *res){
    ht_destroy(&(res->hWords));
    free(res);
}
