#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "tests.h"
#include "count_words.h"
#include "hash_table.h"

void RunTests(){

    {
        CounterResult *res = getstat("Who are you? You ab-cd ?");
        assert(res != NULL);
        //print_inf(res);
        assert(ht_get(&res->hWords, "1") == NULL);
        assert((size_t)ht_get(&res->hWords, "who") == 1);
        assert((size_t)ht_get(&res->hWords, "ab-cd") == 1);
        assert((size_t)ht_get(&res->hWords, "are") == 1);
        assert((size_t)ht_get(&res->hWords, "you") == 2);


        free_res(res);
    }
    printf("\nAll test passed!");

}
