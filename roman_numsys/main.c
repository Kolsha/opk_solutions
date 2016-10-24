#include <stdio.h>
#include <assert.h>

#include "roman_numsys.h"
#define buf_sz 100


int main()
{
    assert(roman_to_decimal("MMMDCCCLXXXVIII") == 3888);

    char *res = sdecimal_to_roman(3888);
    assert(strcmp(res, "MMMDCCCLXXXVIII") == 0);
    free(res);

    res = sdecimal_to_roman(9);
    assert(roman_to_decimal("ix") == 9);
    assert(strcmp(res, "IX") == 0);
    free(res);

    char buf[buf_sz];
    assert(decimal_to_roman(3888, buf, buf_sz) == 1);
    assert(strcmp(buf, "MMMDCCCLXXXVIII") == 0);


    return 0;
}
