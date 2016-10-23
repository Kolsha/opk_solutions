#include "count_words.h"

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
    if(c >= 48 && c <= 57) return 1;
    if(isalnum(c)) return 1;
    return 0;
}

CounterResult *getstat(char *str){

    if(str == NULL){
        return NULL;
    }

    CounterResult *res = (CounterResult*) malloc(sizeof(CounterResult));
    if(res == NULL){
        return NULL;
    }
    res->lines = 1;
    res->symbols = 0;
    res->words = 0;
    res->punctuation = 0;
    res->whitespace = 0;
    res->numbers = 0;

    char p = '\0', c = '\0';

    int prev_space = 1;

    while(str[res->symbols] != '\0')
    {

        c = str[res->symbols];
        res->symbols++;

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

        switch(c){
        case '.':
        case '?':
        case '!':
        case ',':
        case ':':
        case ';':
        case '-':
            res->punctuation++;
            break;
        default:
            break;
        }

        p = c;
        prev_space = isspace(c) > 0;
    }
    if(IsAlphabetic(p)){
        res->words++;
    }
    return res;
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
}