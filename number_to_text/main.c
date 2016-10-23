#include <stdio.h>
#include "converter.h"



int main(int argc, char *argv[])
{
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
