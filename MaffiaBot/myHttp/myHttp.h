#ifndef MY_HTTP_H
#define MY_HTTP_H
#include <stdlib.h>

#define HEADER_LINE_LEN 512


typedef struct httpclient{

    size_t timeout;
    char accept[HEADER_LINE_LEN];
    char user_agent[HEADER_LINE_LEN];
    char referer[HEADER_LINE_LEN];
    int state;
    int error_num;

} httpclient;

int http_init();

char *http_get(char *url);

char *http_post(char *url, char *data);

char *http_last_error();

void http_free();



#endif
