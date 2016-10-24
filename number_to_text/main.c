#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "converter.h"

int main(int argc, char *argv[])
{
    assert(strcmp(dig2str(0, 0, "рубль", "рубля", "рублей"), "ноль рублей") == 0);

    assert(strcmp(dig2str(1009, 0, "рубль", "рубля", "рублей"), "одна тысяча девять рублей") == 0);

    assert(strcmp(dig2str(2, 0, "рубль", "рубля", "рублей"), "два рубля") == 0);

    assert(strcmp(dig2str(1, 1, "монета", "монеты", "монет"), "одна монета") == 0);

    assert(strcmp(dig2str(5, 0, "рубль", "рубля", "рублей"), "пять рублей") == 0);

    char *res = dig2str(446744073709551615UL, 0, "рубль", "рубля", "рублей");
    assert(strcmp(res, "четыреста сорок шесть квадриллионов семьсот сорок четыре триллиона семьдесят три миллиарда семьсот девять миллионов пятьсот пятьдесят одна тысяча шестьсот пятнадцать рублей") == 0);

    int n;
    if (!read_value("Enter number", "%d", &n)) {
        fprintf(stderr, "ERROR: cannot read input\n");
        exit(1);
        return 1;
    }
    FILE *file;
    file = fopen(argv[1], "w");
    if(file==NULL)
    {
        fprintf(stderr, "ERROR: cannot read create file {%s}\n", argv[1]);
        exit(1);
    }
    fprintf(file, "%s", dig2str(n, 0, "рубль", "рубля", "рублей"));
    fclose(file);
    fprintf(stdout, "Complete\n");
    exit(0);
    return 0;
}
