#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "calc.h"

char *GetTextFrom(FILE *input){

    size_t inc = 16;
    size_t str_len = 0;
    size_t buf_size = inc;
    char *buf = malloc(buf_size * sizeof(char));
    if (buf == NULL) {
        return NULL;
    }

    if (input == NULL) {
        return NULL;
    }

    for (;;) {
        // read next part
        if (fgets(buf + str_len, buf_size - str_len, input) == NULL || feof(input)) {
            if (feof(input) && str_len > 0) {
                // read all given input
                break;
            } else {
                // no input at all or some error
                free(buf);
                return NULL;
            }
        }

        size_t added_len = strlen(buf + str_len);
        if (added_len == 0) {
            // there is some input but its length is zero
            // (e.g. "\0foo\n")
            // treat it as error
            free(buf);
            return NULL;
        }
        str_len += added_len;
        assert(str_len + 1 <= buf_size);
        assert(buf[str_len] == '\0');

        if ((str_len + 1 < buf_size) ||     // read less than maximum
                (buf[str_len - 1] == '\n')) {   // read '\n' as last character
            break;
        }

        // otherwise resize buffer for reading next part
        inc++;
        size_t new_buf_size = buf_size + inc;
        if (new_buf_size <= buf_size) {
            // integer overflow, we will be out of memory
            free(buf);
            return NULL;
        }
        char *new_buf = realloc(buf, new_buf_size);
        if (new_buf == NULL) {
            free(buf);
            return NULL;
        }
        buf_size = new_buf_size;
        buf = new_buf;
    }

    assert(str_len > 0);

    // trim trailing '\n' if any
    if (buf[str_len - 1] == '\n') {
        buf[str_len - 1] = '\0';
    }

    // cut buffer to actual length
    char *result = realloc(buf, str_len + 1);
    if (result == NULL) {
        free(buf);
        return NULL;
    }
    return result;
}

static int ismath(char c){
    if(c == '+' || c == '-' || c == ' '
            || c == '*' || c == '/'){
        return c;
    }
    return 0;
}
static char *copystr(const char *str, size_t count){

    if(str == NULL || count < 1){
        return NULL;
    }
    char *res = malloc((count + 1) *sizeof(char));
    if(res == NULL){
        return NULL;
    }
    res[count] = 0;
    for(size_t i = 0; i < count; i++){
        if(str[i] == 0){
            break;
        }
        res[i] = str[i];
    }
    return res;
}

static void destroy_stack(Stack *st){
    Pointer tmp;
    while((tmp = stack_pop(st)) != NULL){
        free(tmp);
    }
    stack_destroy(st);
}

char *postfix_calc(char *exp){

    if(exp == NULL){
        return nsadd(NULL, NULL);
    }

    char math = 0;
    size_t i = 0;
    size_t pos = 0;
    Stack eStack;

    assert(stack_create(&eStack) == 1);
    assert(stack_tune(&eStack, 3, 3) == 1);
    char cur = 0, prev = 0;
    while(exp[i] != 0){

        cur = exp[i];
        if(cur == ' ' && prev ==' '){
            i++;
            pos = i;
            continue;
        }
        math = ismath(exp[i]);


        if(!(math || isdigit(exp[i]))){
            destroy_stack(&eStack);
            return NULL;
        }
        char *number;

        if(math && pos < i){
            number = copystr(&exp[pos], (i - pos));
            if(ismath(exp[pos]) && ismath(exp[i]) && exp[i] != ' '){
                free(number);
                pos = i;
                continue;
            }
            if(number == NULL){
                return nsadd(NULL, NULL);
            }
            assert(stack_push(&eStack, (Pointer)number) == 1);

            pos = i + 1;
        }
        if(math && stack_size(&eStack) > 1 && math != ' '){

            char *number1 = stack_pop(&eStack);
            number = stack_pop(&eStack);

            if(number == NULL || number == NULL){
                stack_destroy(&eStack);
                return NULL;
            }
            char *tmp;
            switch (math) {
            case '+':
                tmp = nsadd(number, number1);
                break;
            case '-':
                tmp = nssub(number, number1);
                break;
            case '*':
                tmp = nsmul(number, number1);
                break;
            case '/':
                tmp = nsdiv(number, number1);
                break;
            }
            free(number);
            free(number1);
            if(tmp == NULL){
                stack_destroy(&eStack);
                return NULL;
            }
            assert(stack_push(&eStack, (Pointer)tmp) == 1);
            pos = i + 1;

        }
        i++;
        prev = cur;
    }


    if(stack_size(&eStack) ==  1){
        Pointer tmp;
        tmp = stack_pop(&eStack);
        stack_destroy(&eStack);
        return tmp;
    }else if(stack_size(&eStack) > 1){
        destroy_stack(&eStack);
    }
    return NULL;

}
