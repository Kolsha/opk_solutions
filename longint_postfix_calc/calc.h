#ifndef CALC_H
#define CALC_H

#include <stdlib.h>
#include "mathstr.h"
#include "../MyStack/mystack.h"

char *GetTextFrom(FILE *input);
char *postfix_calc(char *exp);

#endif
