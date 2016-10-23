#include "roman_numsys.h"

static int get_value_by_roman(char c){
    switch (c) {
    case 'I':
        return 1;
        break;
    case 'V':
        return 5;
        break;
    case 'X':
        return 10;
        break;
    case 'L':
        return 50;
        break;
    case 'C':
        return 100;
        break;
    case 'D':
        return 500;
        break;
    case 'M':
        return 1000;
        break;
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
int decimal_to_roman(int number, char *string, int maxlen)
{
    /*
     * Max : MMMDCCCLXXXVIII
     */
    if(number < 0 || number > 4000
            || maxlen < 1 || string ==NULL){
        return 0;
    }
    string[maxlen - 1] = '\0';

    int  decimalValue[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    char *romanNumeral[] = { "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    size_t pos = 0;

    for (unsigned int i = 0; i < (sizeof(decimalValue) / sizeof(int)); i++) {
        while (decimalValue[i] <= number) {
            size_t rpos = 0;
            char *r = romanNumeral[i];
            while(r[rpos] != 0){
                string[pos++] = r[rpos];
                rpos++;
                if((pos + 2) == (unsigned)maxlen){
                    return 1;
                }
            }
            //romanized += romanNumeral[index];
            number -= decimalValue[i];
        }
    }
    string[pos] = '\0';

    return 1;
}
