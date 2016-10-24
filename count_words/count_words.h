#ifndef COUNT_WORDS
#define COUNT_WORDS

#include <stdio.h>

typedef struct _CounterResult {
    size_t symbols;
    size_t words;
    size_t lines;
    size_t punctuation;
    size_t whitespace;
    size_t numbers;
} CounterResult;

char *GetTextFrom(FILE *input);

CounterResult *getstat(char *str);

void print_inf(CounterResult *res);


#endif
