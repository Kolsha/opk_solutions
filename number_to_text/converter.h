#ifndef NUMBER_TO_TEXT
#define NUMBER_TO_TEXT

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DG_POWER 6

int read_value(const char *request, const char *fmt, void *value);

char *dig2str (unsigned long long sum, int sex, char *one, char *four, char *many);

#endif
