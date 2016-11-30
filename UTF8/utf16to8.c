#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utf16to8.h"

static void print_bits(unsigned num){

    for(int c = 31; c >= 0; c--){
        int bit = (num >> c) & 1;
        printf("%d", bit);
    }

    printf("\n");
}

char *codepoint_to_utf8b(unsigned cp){
    /*
 * http://stackoverflow.com/questions/6240055/manually-converting-unicode-codepoints-into-utf-8-and-utf-16
 */
    if(cp == 0){
        return strdup("\x0");
    }

    int i;
    for(i = 31; i >= 0;i--){
        if( ((cp >> i) & 1) == 1)
        {
            break;
        }
    }

    unsigned res = 0;

    if(i <= 6){
        res = cp;
    }else if(i <= 10){
        res = 0b00000000000000001100000010000000;
    }else if(i <= 15){
        res = 0b00000000111000001000000010000000;
    }else{
        res = 0b11110000100000001000000010000000;
    }

    int pos = 0;
    if(i > 6){
        for(int c = 0; c <= 31; c++){
            int bit = (cp >> c) & 1;
            if(pos >= 26 || c > i){
                break;
            }
            res ^= (-bit ^ res) & (1 << pos);

            if(i > 6 && pos == 5){
                pos += 2;
            }else if(i > 10 && (pos == 13 || pos == 21)){
                pos += 2;
            }
            pos++;
        }
    }

    char utf8b[5] = {0};
    pos = 0;
    for(i = 3; i >= 0; i--){
        char byte = ((unsigned char *)(&res))[i];
        if(byte == 0){
            continue;
        }
        utf8b[pos++] = byte;
    }
    return (i == 0) ? NULL : strdup(utf8b);
}

static unsigned get_next_surrogate(char *utf16s, size_t *pos){

    char hex[5] = {0};

    if(utf16s == NULL || pos == NULL){
        return 0;
    }
    size_t my_pos = *pos;
    if(utf16s[my_pos] == '\\'){
        my_pos++;
    }
    if(utf16s[my_pos] == 'u'){
        my_pos++;
    }
    int i = 0;
    while(i < 4 && utf16s[my_pos] != 0){
        hex[i++] = utf16s[my_pos++];
    }
    *pos = my_pos;
    if(i < 4){
        return 0;
    }

    unsigned res = (unsigned) strtol(hex, NULL, 16); // &hex

    return res;
}

char *utf16s_to_utf8b(char *utf16s){

    if(utf16s == NULL){
        return NULL;
    }

    size_t len_str = strlen(utf16s);
    if(len_str < 6){// "\uFFFF"
        return strdup(utf16s);
    }

    size_t res_len = 4 * len_str / 3 + 1;
    char *res = (char*) malloc(res_len * sizeof(char));
    if(res == NULL){
        return NULL;
    }
    res[0] = '\0';

    size_t res_pos = 0;
    //size_t escapes = 0;
    size_t pos = 0;
    char cur = 0;
    char prev = 0;
    while((cur = utf16s[pos]) != 0 || prev != '\0'){
        /*if(cur == '\\'){
            escapes = (prev == cur) ? (escapes + 1) : 1;
        }else{
           escapes = 0;
        }*/
        if(cur == 'u' && prev == '\\'){
            char *utf8b = NULL;
            unsigned lead = get_next_surrogate(utf16s, &pos);

            if(lead < 0xD800 || lead > 0xDFFF){
                utf8b = codepoint_to_utf8b(lead);
            }else if(lead >= 0xDC00){
                utf8b = NULL;
            }else{

                unsigned tail = get_next_surrogate(utf16s, &pos);
                unsigned codepoint = ((lead & 0x3FF)  << 10);

                if ((tail < 0xDC00) || (tail > 0xDFFF)){
                    utf8b = NULL;
                }else
                {
                    codepoint = codepoint | ((tail & 0x3FF));
                    codepoint = (codepoint + 0x10000);
                    //printf("%X\n", codepoint);
                    utf8b = codepoint_to_utf8b(codepoint);
                }
            }

            if(utf8b != NULL){
                size_t utf8_len = strlen(utf8b);
                //printf("%s\n", utf8b);
                if(utf8_len > 0){
                    if((res_len <= res_pos) ||
                            (res_len - res_pos) < utf8_len){

                        size_t new_res_len = res_len * 4 / 3 + 1;
                        char *tmp = realloc(res, new_res_len);
                        if(tmp == NULL){
                            free(res);
                            free(utf8b);
                            return NULL;
                        }
                        res = tmp;
                        res_len = new_res_len;
                    }

                    if(strcat(res, utf8b) != NULL){
                        res_pos += utf8_len;
                        res[res_pos] = '\0';
                    }

                }
                free(utf8b);
            }
        }else if(prev != '\0'){
            if((res_pos + 1) >= res_len){
                size_t new_res_len = res_len * 4 / 3 + 1;
                char *tmp = realloc(res, new_res_len);
                if(tmp == NULL){
                    free(res);
                    return NULL;
                }
                res = tmp;
                res_len = new_res_len;
            }
            res[res_pos++] = prev;
            res[res_pos] = '\0';
        }
        cur = utf16s[pos];
        prev = cur;
        if(prev == cur && prev == '\0'){
            break;
        }
        pos++;
    }
    if(res == NULL){
        return NULL;
    }
    res_pos = strlen(res) + 1;
    if(res_pos < res_len){
        char *tmp = realloc(res, res_pos);
        if(tmp != NULL){
            res = tmp;
        }
    }

    return res;

}



