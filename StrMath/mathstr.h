#ifndef MATH_STR_H
#define MATH_STR_H

#include <stdio.h>

typedef struct _StrNumber{
    int sign;
    int *n;
    int count;
} StrNumber;

StrNumber* readnumber(const char *n);
StrNumber* nsadd_raw(StrNumber *a, StrNumber *b);
void print_arr(int *arr, size_t size);
char* makenumber(StrNumber *numb);


const char* nsadd(const char*, const char*);
const char* nssub(const char*, const char*);
const char* nsmul(const char*, const char*);
const char* nsdiv(const char*, const char*);
const char* nsmod(const char*, const char*);



#endif
