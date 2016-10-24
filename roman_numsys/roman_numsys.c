#include "roman_numsys.h"

/*
 * Max : MMMDCCCLXXXVIII - 3888
 */

#define max_sz 15

static int get_value_by_roman(char c){
    switch (c) {
    case 'I':
        return 1;

    case 'V':
        return 5;

    case 'X':
        return 10;

    case 'L':
        return 50;

    case 'C':
        return 100;

    case 'D':
        return 500;

    case 'M':
        return 1000;

    }
    return 0;
}

int roman_to_decimal(char *str_roman)
{
    if(str_roman == NULL){
        return -1;
    }

    size_t i = 0;
    size_t res = 0;
    char cur = 0;
    char prev = 0;

    while(str_roman[i] != 0){
        cur = toupper(str_roman[i]);

        int tmp_cur = get_value_by_roman(cur);
        res += tmp_cur;

        int tmp_prev = get_value_by_roman(prev);
        if(tmp_cur > tmp_prev){
            res -= 2 * tmp_prev;
        }
        prev = cur;
        i++;

    }
    return res;
}

char *sdecimal_to_roman(int number){

    if(number < 0 || number > 4000){
        return NULL;
    }
    char str[max_sz + 1] = {0};
    int  decimalValue[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    char *romanNumeral[] = { "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    size_t pos = 0;

    for (unsigned int i = 0; i < (sizeof(decimalValue) / sizeof(int)); i++) {
        while (decimalValue[i] <= number) {
            size_t rpos = 0;
            char *r = romanNumeral[i];
            while(r[rpos] != 0){
                str[pos++] = r[rpos];
                rpos++;
                if((pos + 1) == max_sz){
                    break;
                }
            }
            number -= decimalValue[i];
        }
    }
    char *res = malloc(pos * sizeof(char));
    if(res == NULL){
        return NULL;
    }

    for(size_t i = 0; i < pos + 1; i++){
        res[i] = str[i];
    }

    return res;

}

int decimal_to_roman(int number, char *string, int maxlen)
{

    if(number < 0 || number > 4000
            || maxlen < 1 || string ==NULL){
        return 0;
    }
    string[maxlen - 1] = '\0';
    char *res = sdecimal_to_roman(number);
    if(res == NULL){
        return 0;
    }
    size_t pos = 0;

    while (res[pos] != 0){
        string[pos] = res[pos];
        pos++;
        if((pos + 2) == maxlen){
            return 1;
        }
    }
    string[pos] = '\0';

    return 1;
}

