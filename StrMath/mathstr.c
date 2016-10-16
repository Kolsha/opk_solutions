#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include "mathstr.h"
#define BASE 10
#define MAX_NUMB_IN_INT 20

typedef struct _StrNumber{
    int sign;
    int *n;
    int count;
} StrNumber;

/*
 * nscompare(n1, n2):
 * a > b =  1
 * a = b =  0
 * a < b = -1
 * Error =  2
 */
static int nscompare(StrNumber *a, StrNumber *b, int abs){

    if(a->n == NULL || b->n == NULL){
        return 2;
    }
    if(abs != 1){
        if(a->sign > b->sign){
            return 1;
        } else if(a->sign < b->sign){
            return -1;
        }
    }
    if(a->count > b->count){
        return 1;
    } else if(a->count < b->count){
        return -1;
    }
    for(int i = b->count - 1; i >= 0; i--){
        if(a->n[i] > b->n[i]){
            return 1;
        }else if(a->n[i] < b->n[i]){
            return -1;
        }
    }
    return 0;
}

void print_arr(int *arr, size_t size){

    for(size_t i = 0; i < size; i++){
        printf("%d ",arr[i]);
    }
    printf("\n");
}

static int max(int a, int b){
    if(a > b){
        return a;
    }
    return b;
}

static StrNumber* readnumber(const char* n){


    if(n == NULL){
        return NULL;
    }
    StrNumber *res = (StrNumber*) malloc(sizeof(StrNumber));
    if(res == NULL){
        return NULL;
    }
    res->sign = 0;
    res->count = 0;

    int i = 0;
    int lenstr = 0;
    while(n[i] != 0){
        lenstr++;
        if(n[i] == '0' && !res->count){
            i++;
            continue;
        }
        if(isdigit(n[i])){
            res->count++;
        }
        if((n[i] == '-' || n[i] == '+') && res->sign == 0){
            res->sign = 1;
            if(n[i] == '-')
            {
                res->sign = -1;
            }
        }
        i++;
    }
    if(res->count < 1){
        return NULL;
    }
    if(res->sign == 0){
        res->sign = 1;
    }
    res->n = (int*) malloc(res->count * sizeof(int));
    if(res->n == NULL){
        return NULL;
    }
    i = lenstr;
    int c = 0;
    while(i > 0 && i--){
        if(!isdigit(n[i])){
            continue;
        }
        if(c >= res->count){
            break;
        }
        res->n[c] = (int)(n[i] - '0');
        c++;
    }

    return res;
}

static void freenumber(StrNumber *numb){
    if(numb == NULL) {
        return ;
    }
    free(numb->n);
    free(numb);
}

static int copynumb(StrNumber *in, StrNumber *source){
    if(in->count != source->count ||
            in->n == NULL || source->n == NULL){
        return 0;
    }
    in->count = source->count;
    in->sign = source->sign;
    for(int i = 0; i < source->count; i++){
        in->n[i] = source->n[i];
    }
    return 1;
}

static char* MakeZero(){
    char *res = malloc(2);
    if(res == NULL){
        return NULL;
    }
    res[0] = '0';
    res[1] = 0;
    return res;
}

static char* makenumber(StrNumber *numb){
    if(numb == NULL || numb->n == NULL || numb->count < 1){
        return NULL;
    }
    int c = numb->count + 1;
    if(numb->sign < 0){
        c++;
    }
    char *res = malloc(c);
    if(res == NULL){
        return NULL;
    }
    res[c - 1] = 0;

    for(int i = 0; i < c; i++){
        res[c - i - 2] = (char)(numb->n[i] + '0');
        if(!isdigit(res[c - i - 2])){
            res[c - i - 2] = '?';
        }
    }
    if(numb->sign < 0){
        res[0] = '-';
    }

    return res;
}

static StrNumber* nsadd_raw(StrNumber *a, StrNumber *b){
    if(a == NULL || b == NULL ||
            a->n == NULL || b->n == NULL){
        return NULL;
    }
    int c = max(a->count, b->count);
    if(c < 1){
        return NULL;
    }
    if((a->sign * a->count) < (b->sign * b->count)){
        StrNumber *tmp = a;
        a = b;
        b = tmp;
        tmp = NULL;
    }

    StrNumber *res = (StrNumber*) malloc(sizeof(StrNumber));
    if(res == NULL){
        return NULL;
    }
    res->sign = 0;
    res->count = 1;

    res->n = (int*) malloc((1 + c) * sizeof(int));
    if(res->n == NULL){
        freenumber(res);
        return NULL;
    }
    res->count = c + 1;

    for(int i = 0; i < res->count; i++){
        res->n[i] = 0;
    }

    int carry = 0;
    for(int i = 0; i < c; i++){

        int t = carry;
        if (a->count > i){
            t += a->sign * a->n[i];
        }
        if (b->count > i){
            t += b->sign * b->n[i];
        }

        carry = t / BASE;
        res->n[i] += t % BASE;

        if(res->n[i] < 0 && a->sign != b->sign){
            if(a->count > b->count){
                res->n[i + 1]--;
                res->n[i] += BASE;
            }else{
                res->n[i] *= -1;
                res->sign = -1;
            }

        }else if(a->sign == b->sign){
            res->sign = a->sign;
            res->n[i] *= a->sign;
        }
        //print_arr(res->n, res->count);

    }
    res->n[ res->count - 1] = 0;
    if(carry > 0){
        res->n[ res->count - 1] = carry;
    }else if(carry < 0){
        res->sign = -1;
        res->n[ res->count - 1] = res->sign * carry;
    }
    carry = res->count - 1;
    while(res->n[carry] == 0 && carry > 0){
        carry--;
    }
    carry++;

    free(&res->n[carry]);
    res->count = carry;

    if(res->sign == 0){
        res->sign = 1;
    }
    return res;

}

static StrNumber* nsmul_raw(StrNumber *a, StrNumber *b){
    if(a == NULL || b == NULL ||
            a->n == NULL || b->n == NULL){
        return NULL;
    }

    int c = a->count * b->count + 1;
    if(c < 1){
        return NULL;
    }


    StrNumber *res = (StrNumber*) malloc(sizeof(StrNumber));
    if(res == NULL){
        return NULL;
    }

    res->sign = a->sign * b->sign;
    res->count = 1;

    res->n = (int*) malloc(c * sizeof(int));
    if(res->n == NULL){
        freenumber(res);
        return NULL;
    }
    res->count = c;

    for(int i = 0; i < res->count; i++){
        res->n[i] = 0;
    }

    for (int i = 0; i < a->count; i++)
    {
        for (int j = 0; j < b->count; j++)
        {
            res->n[i + j] += abs(a->n[i] * b->n[j]);
        }

    }

    for (int i = 0; i < c; i++)
    {
        res->n[i + 1] +=  res->n[i] / BASE;
        res->n[i] %= BASE;
    }
    c = res->count - 1;
    while(res->n[c] == 0 && c > 0){
        c--;
    }
    c++;

    free(&res->n[c]);
    res->count = c;
    return res;

}

static void inttonumb(int i, StrNumber *numb){
    if(numb->n == NULL){
        return ;
    }
    numb->sign = 1;
    if(i < 0){
        numb->sign = -1;
    }
    int c = 0;
    while(i > 0){
        numb->n[c] = (i % BASE) * numb->sign;
        i /= BASE;
        c++;
    }
    numb->count = c;
}

static void incdecnumb(StrNumber *numb, int sign){
    StrNumber n, *res;
    int a[1] = {1};
    n.sign = 1;
    if(sign < 0){
        n.sign = -1;
    }
    n.count = 1;
    n.n = a;
    res = nsadd_raw(numb, &n);

    if(res == NULL){
        return ;
    }
    free(numb->n);
    numb->n = res->n;
    numb->count = res->count;
    free(res);
}

static StrNumber* nsdiv_raw(StrNumber *a, StrNumber *b){
    if(a == NULL || b == NULL ||
            a->n == NULL || b->n == NULL){
        return NULL;
    }

    int c = a->count;
    if(c < 1 || (b->count == 1 && b->n[0] == 0)){
        return NULL;
    }


    StrNumber *res = (StrNumber*) malloc(sizeof(StrNumber));
    if(res == NULL){
        return NULL;
    }

    res->sign = a->sign * b->sign;
    res->count = 1;

    res->n = (int*) malloc(c * sizeof(int));
    if(res->n == NULL){
        freenumber(res);
        return NULL;
    }
    res->count = c;
    if(b->count == 1 && b->n[0] == 1){
        if (copynumb(res, a)){
            return res;
        }
        freenumber(res);
        return NULL;
    }


    for(int i = 0; i < res->count; i++){
        res->n[i] = 0;
    }

    int cmp = nscompare(a, b, 1);
    if(cmp > 1){
        freenumber(res);
        return NULL;
    }

    if((a->count == 1 && a->n[0] == 0) || cmp < 1){
        res->count = 1;
        res->n[0] = 0;
        if(cmp == 0 && !(a->count == 1 && a->n[0] == 0)){
            res->n[0] = 1;
        }
        free(&res->n[1]);
        return res;
    }

    StrNumber mult, *comp;
    mult.n = (int*) malloc(sizeof(int));
    mult.count = 1;
    mult.sign = 1;
    if(mult.n == NULL){
        freenumber(res);
        return NULL;
    }
    mult.n[0] = 2;
    comp = nsmul_raw(b, &mult);
    while((cmp = nscompare(a, comp, 1)) > 0){
        freenumber(comp);
        incdecnumb(&mult, 1);
        comp = nsmul_raw(b, &mult);
        print_arr(comp->n, comp->count);
    }
    freenumber(comp);
    if(cmp < 0){
        incdecnumb(&mult, -1);
    }
    res->count = mult.count;
    mult.sign = res->sign;

    free(&res->n[mult.count]);
    copynumb(res, &mult);

    freenumber(&mult);

    return res;

}




const char* nsadd(const char *a, const char *b){
    StrNumber *an = readnumber(a);
    StrNumber *bn = readnumber(b);
    if(an == NULL || bn == NULL){
        return MakeZero();
    }
    StrNumber *res = nsadd_raw(an, bn);
    if(res == NULL){
        return MakeZero();
    }
    char *str = makenumber(res);

    freenumber(res);
    freenumber(an);
    freenumber(bn);

    return str;
}

const char* nssub(const char *a, const char *b){
    StrNumber *an = readnumber(a);
    StrNumber *bn = readnumber(b);
    if(an == NULL || bn == NULL){
        return MakeZero();
    }
    bn->sign *= -1;
    StrNumber *res = nsadd_raw(an, bn);
    if(res == NULL){
        return MakeZero();
    }
    char *str = makenumber(res);

    freenumber(res);
    freenumber(an);
    freenumber(bn);

    return str;
}

const char* nsmul(const char *a, const char *b){
    StrNumber *an = readnumber(a);
    StrNumber *bn = readnumber(b);
    if(an == NULL || bn == NULL){
        return MakeZero();
    }
    StrNumber *res = nsmul_raw(an, bn);
    if(res == NULL){
        return MakeZero();
    }
    char *str = makenumber(res);

    freenumber(res);
    freenumber(an);
    freenumber(bn);

    return str;
}

const char* nsdiv(const char *a, const char *b){
    StrNumber *an = readnumber(a);
    StrNumber *bn = readnumber(b);
    if(an == NULL || bn == NULL){
        return MakeZero();
    }

    StrNumber *res = nsdiv_raw(an, bn);
    if(res == NULL){
        return MakeZero();
    }
    char *str = makenumber(res);

    freenumber(res);
    freenumber(an);
    freenumber(bn);

    return str;
}
