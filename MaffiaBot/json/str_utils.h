#ifndef STR_UTILS_H
#define STR_UTILS_H

#include <stdlib.h>
#include <time.h>

#define _LOG_FILE_NAME_ "log.txt"

char get_next_type(char *str, size_t *pos);

char *copystr(const char *str, size_t count);

char *get_str(char *str, size_t *pos);

void _init_log_();

void _Log_(char *frmt, ...);

void _cleanup_log_();

char *my_strcat(char *s1, char *s2);

char *my_strcpy(char *dest, char *src, size_t dest_sz);

size_t my_strlen(char *s);

int go_to_next(char *str, size_t *pos);

char *my_strstr(char *haystack,
                char *needle);

int mRand(int min, int max);

char *frmt_time(time_t tm);

char *build_request(char *frmt, ...);

#endif
