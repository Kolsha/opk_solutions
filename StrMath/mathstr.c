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

void print_numb(StrNumber *numb){

    for(size_t i = 0; i < numb->count; i++){
        printf("%d ",numb->n[i]);
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
    if(carry < res->count)
    {
        free(&res->n[carry]);
    }
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
    if(numb->n != NULL)
    {
        free(numb->n);
    }
    numb->n = res->n;
    numb->count = res->count;
    free(res);
}

static int pusnton(int N, StrNumber *numb){
    if(numb->n == NULL && numb->count != 0){
        return 0;
    } else if(!numb->count && !N){
        return 1;
    }
    int *arr = malloc((numb->count + 1) * sizeof(int));
    if(arr == NULL){
        return 0;
    }
    int i = 0;
    arr[i++] = N;
    while(i <= numb->count){
        arr[i] = numb->n[i - 1];
        i++;
    }
    numb->count++;
    free(numb->n);
    numb->n = arr;
    return 1;
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

    StrNumber div;
    div.count = b->count;
    div.sign = 1;
    div.n = (int*) malloc(b->count * sizeof(int));

    if(div.n == NULL){
        freenumber(res);
        return NULL;
    }

    for(int i = 0; i < b->count; i++){
        div.n[b->count - i - 1] = a->n[a->count - i - 1];
    }

    int apos = b->count - 1;
    int respos = 0;

    while(2 < 3){
        int addcounter = 0;
        while(nscompare(b, &div, 1) == 1){
            apos++;
            addcounter++;
            if(apos == a->count){
                break;
            }

            if(!pusnton(a->n[a->count - apos - 1], &div))
            {

                freenumber(res);
                freenumber(&div);
                return NULL;
            }
            if(addcounter > 1){
                res->n[respos++] = 0;
            }

        }

        if(apos == a->count){
            if(div.count == 0 || (div.count == 1 && !div.n[0])){
                res->n[respos++] = 0;
            }
            break;
        }


        StrNumber mult ={1, NULL, 1};

        mult.n = (int*) malloc(sizeof(int));
        if(mult.n == NULL){
            freenumber(res);
            free(div.n);
            return NULL;
        }
        mult.n[0] = 2;

        StrNumber *comp = nsmul_raw(b, &mult);

        while((cmp = nscompare(&div, comp, 1)) > 0){
            freenumber(comp);
            incdecnumb(&mult, 1);
            comp = nsmul_raw(b, &mult);
        }


        if(cmp < 0){
            incdecnumb(&mult, -1);
            comp = nsmul_raw(b, &mult);

        }
        for(int i = 0; i < mult.count; i++)
        {
            res->n[respos++] = mult.n[i];
        }

        comp->sign = -1;
        StrNumber *tmp = nsadd_raw(&div, comp);


        /*printf("Circle:\n\tdiv:");
        print_numb(&div);
        printf("\ttmp:");
        print_numb(tmp);
        printf("\tres:");
        print_numb(res);*/

        freenumber(comp);
        free(mult.n);
        free(div.n);

        if(nscompare(b, tmp, 1) == 1 && apos == (a->count - 1)){
            //printf("Mod:");
            //print_numb(tmp);
            freenumber(tmp);
            break;
        }
        div.count = tmp->count;
        div.n = tmp->n;

        if(tmp->count == 1 && !tmp->n[0]){
            free(div.n);
            div.count = 0;
        }
        free(tmp);

    }
    res->count = respos;
    free(&res->n[respos]);
    for(int i = 0; i < (res->count / 2); i++){
        respos = res->n[i];
        res->n[i] = res->n[res->count - i - 1];
        res->n[res->count - i - 1] = respos;
    }

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
