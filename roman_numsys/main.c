#include <stdio.h>

#include "roman_numsys.h"













int main(int argc, char *argv[])
{
    char buf[100];
    if(decimal_to_roman(1111,buf, 100) == 1)
    {
        printf("%s", buf);
    }
    return 0;
}
