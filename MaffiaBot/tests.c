#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "tests.h"

#include "json/hash_table.h"
#include "json/json.h"
#include "json/str_utils.h"

#include "myHttp/myHttp.h"
#include "json/utf16to8.h"

static void Utf8Test(){

    char *in[] = {"Test", "\\Test", "\\u0421",
                  "\\uD83D\\uDE33", "\\u2714",
                  "\\u0038\\u20E3", "\\u261D",
                  "This is Russia: \\uD83C\\uDDF7\\uD83C\\uDDFA", NULL};

    char *out[] = {"Test", "\\Test", "\xD0\xA1",
                   "\xF0\x9F\x98\xB3", "\xE2\x9C\x94",
                   "\x38\xE2\x83\xA3", "\xE2\x98\x9D",
                   "This is Russia: \xF0\x9F\x87\xB7\xF0\x9F\x87\xBA", NULL};

    int i = 0;

    while(in[i] != NULL && out[i] != NULL){

        char *tmp = utf16s_to_utf8b(in[i]);
        assert(tmp != NULL);
        assert(strcmp(tmp, out[i]) == 0);
        free(tmp);

        i++;
    }

}

static void HashTableTest(){

    HashTable ht;

    assert(ht_init(&ht, 100, NULL, NULL) == 1);


    assert(ht_set(&ht, "Vasya", (Pointer)"333-45-67") != NULL);
    assert(ht_set(&ht, "Police", (Pointer)"02") != NULL);

    assert(ht_count(&ht) == 2);

    assert(ht_resize(&ht, 50));

    assert(ht_count(&ht) == 2);

    assert(!strcmp(ht_get(&ht, "Vasya"), "333-45-67"));

    assert(ht_has(&ht, "Police"));

    assert(ht_set(&ht, "Police1", (Pointer)"022") != NULL);
    assert(ht_set(&ht, "Police2", (Pointer)"023") != NULL);

    assert(ht_count(&ht) == 4);

    ht_delete(&ht, "Police1");

    assert(ht_count(&ht) == 3);

    ht_destroy(&ht);

}

static void JsonTest(){

    JSONObj *json =
            json_parse("{\"a\":\"abcde\", \"b\": 1234, \"c\":{\"d\": 1234}, \"array\":[1234]}");
    JSONObj *tmp;
    char *str;
    assert(json != NULL);

    str = json_get_str(json, "a");
    assert(strcmp(str, "abcde") == 0);

    str = json_get_str(json, "b");
    assert(strcmp(str, "1234") == 0);

    tmp = json_get(json, "c");

    assert(tmp != NULL);

    str = json_get_str(tmp, "d");
    assert(strcmp(str, "1234") == 0);

    tmp = json_get(json, "array");
    assert(tmp != NULL);
    JSONList *arr = json_arr_list(tmp);
    assert(arr != NULL && arr->data != NULL);
    json_free(json);
}

static void mHttpTest(){

    char *res = http_get("https://api.telegram.org/bot1234:1234/GetMe");
    assert(res != NULL);
    assert(http_last_error() == NULL);
    assert(my_strstr(res, "Unauthorized"));
    free(res);

}

void RunTests(){

    HashTableTest();
    JsonTest();
    mHttpTest();
    Utf8Test();
    _Log_("All test passed!");

}
