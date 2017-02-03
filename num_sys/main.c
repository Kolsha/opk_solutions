#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "converter.h"

char * read_line(const char * request) {
    size_t str_len = 0;
    size_t buf_size = 16; // some initial size
    char *buf = malloc(buf_size * sizeof(char));
    if (buf == NULL) {
        return NULL;
    }

    if (request != NULL) {
        printf("%s: ", request);
    }

    for (;;) {
        // read next part
        if (fgets(buf + str_len, buf_size - str_len, stdin) == NULL) {
            if (feof(stdin) && str_len > 0) {
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

        // check if we finished with reading
        if ((str_len + 1 < buf_size) ||     // read less than maximum
                (buf[str_len - 1] == '\n')) {   // read '\n' as last character
            break;
        }

        // otherwise resize buffer for reading next part
        size_t new_buf_size = buf_size * 2;
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








int main(){
    char *num = read_line("Write number");
    if(num == NULL){
        printf("\nI cry;(");
        return 1;
    }

    int num_sys = -1;
    printf("Write number system: ");
    scanf("%d", &num_sys);
    if(num_sys < 2 || num_sys > 36){
        printf("\nYou stupid");
        return 1;
    }

    DoubleNumber *res = num2dec(num, num_sys);
    if(res == NULL){
        printf("\nSomething wrong, may be you daun.");
        return 1;
    }
    int to_sys = -1;
    printf("Write convert system: ");
    scanf("%d", &to_sys);

    char *num_res = dec2num(res, to_sys);

    if(num_res != NULL){
        printf("Your number in %d: %s", to_sys, num_res);

        free(num_res);
    } else{
        printf("\nSomething wrong, may be you daun.");
        return 1;
    }

    return 0;
}

