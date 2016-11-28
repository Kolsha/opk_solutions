#ifndef STR_UTILS_H
#define STR_UTILS_H
#include <stdlib.h>
#include <time.h>

char get_next_type(char *str, size_t *pos);

char *copystr(const char *str, size_t count);

char *get_str(char *str, size_t *pos);

void _Log_(char *frmt, ...);

char *my_strcat(char *s1, char *s2);

size_t my_strlen(char *s);

int go_to_next(char *str, size_t *pos);

char *my_strstr(char *haystack, char *needle);

int mRand(int min, int max);

char *frmt_str(char *one_frmt, char *one_param);

char *frmt_time(time_t tm);

char *build_request(char *frmt, ...);

#endif
