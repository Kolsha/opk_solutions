#include <stdio.h>
#include "rational.h"

int main()
{
    Rational p, q, r;
    rat_create(&p, -1, 4);
    rat_create(&q, 16, -3);
    int x = rat_compare(&p, &q);

    rat_print(&p, stdout);
    printf("\n");
    rat_print(&q, stdout);
    printf("Reult: %d\n", x);
    return 0;
}
