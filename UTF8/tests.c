#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "tests.h"
#include "utf16to8.h"

void RunTests(){

    char *in[] = {"Test", "\\Test", "\\u0421",
                  "\\uD83D\\uDE33", "\\u2714",
                  "\\u0038\\u20E3", "\\u261D",
                  "This is Russia: \\uD83C\\uDDF7\\uD83C\\uDDFA", NULL};

    char *out[] = {"Test", "\\Test", "\\xD0\\xA1",
                   "\\xF0\\x9F\\x98\\xB3", "\\xE2\\x9C\\x94",
                   "\\x38\\xE2\\x83\\xA3", "\\xE2\\x98\\x9D",
                   "This is Russia: \\xF0\\x9F\\x87\\xB7\\xF0\\x9F\\x87\\xBA", NULL};

    int i = 0;

    while(in[i] != NULL && out[i] != NULL){

        char *tmp = utf16s_to_utf8b(in[i]);
        assert(tmp != NULL);
        assert(strcmp(tmp, out[i]) == 0);
        free(tmp);

        i++;
    }

}
