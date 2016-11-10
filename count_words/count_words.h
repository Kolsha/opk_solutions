#ifndef COUNT_WORDS
#define COUNT_WORDS
#include <stdio.h>
#include "hash_table.h"

typedef struct _CounterResult {
    size_t symbols;
    size_t words;
    size_t lines;
    size_t punctuation;
    size_t whitespace;
    size_t numbers;
    HashTable hWords;
} CounterResult;

char *GetTextFrom(FILE *input);

CounterResult *getstat(char *str);

void print_inf(CounterResult *res);
void free_res(CounterResult *res);

#endif
