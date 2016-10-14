#include "mathstr.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
static void normalize(char* n){
    if(n == NULL){
        return ;
    }
    int i = 0;
    int pos = 0;
    int havenum = 0;
    int havesign = 0;
    while(n[i] != 0){
        if(n[i] == '0' && !havenum){
            i++;
            continue;
        }
        if(isdigit(n[i])){
            havenum = 1;
            n[pos++] = n[i];
            if((pos - 1) != i)
            {
                n[i] = 'A';
            }
        }
        if(n[i] == '-' && !havesign){
            havesign = 1;
            n[pos++] = n[i];
            if((pos - 1) != i)
            {
                n[i] = 'A';
            }
        }
        i++;
    }
    n[pos++] = 0;
    free(&n[pos]);
}

static char sign(int i){
    if(i < 0){
        return '-';
    }
    return '0';
}

static int CheckNumber(const char *n, int *sign, size_t *len){
    *len = strlen(n);
    if(n == NULL || *len < 1){
        return 0;
    }
    *sign = 1;
    int i = 0;
    if(n[0] == '-'){
        *sign = -1;
        i++;
    }

    while(n[i] != 0){
        if(!isdigit(n[i])){
            return 0;
        }
        i++;
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

const char* nsadd(const char *a, const char *b){
    int signa, signb;
    size_t lena, lenb, lenr, offseta, offsetb;
    const int addsize = 3;
    if(!CheckNumber(a, &signa, &lena) || !CheckNumber(b, &signb, &lenb)){
        return MakeZero();
    }
    lenr = lena + addsize;
    offseta = 0;
    offsetb = lenr - lenb - addsize;
    if(lenb > lena){
        lenr = lenb + addsize;
        offsetb = 0;
        offseta = lenr - lena - addsize;
    }
    char *res = malloc(lenr);
    if(res == NULL){
        return MakeZero();
    }
    res[lenr-1] = 0;
    int ost = 0;
    int suma = 0;
    int sumb = 0;
    for(size_t i = (lenr - addsize); i > 0; i--){
        int dA = (((int)(i-offseta) > 0 && isdigit(a[i-offseta - 1])) ? a[i-offseta - 1] : '0');
        int dB = (((int)(i-offsetb) > 0 && isdigit(b[i-offsetb - 1])) ? b[i-offsetb - 1] : '0');
        dA = signa * (dA - '0');
        dB = signb * (dB - '0');
        suma += dA;
        sumb += dB;
        int t = dA + dB + ost;

        res[i + 1] = abs(t % 10) + '0';
        ost = t / 10;
    }
    res[1] = ost + '0';
    if(suma > sumb){
        res[0] = sign(signa);
    }else if(sumb > suma){
        res[0] = sign(signb);
    }
    normalize(res);
    return res;

}

const char* nssub(const char *a, const char *b){
    size_t lenb = strlen(b);
    if(b == NULL || lenb < 1){
        return MakeZero();
    }
    int needsign = 0;
    if(b[0] != '-'){
        lenb++;
        needsign = 1;
    }
    char *btmp = malloc(lenb);
    if(btmp == NULL){
        return MakeZero();
    }
    if(needsign){
        btmp[0] = '-';
    }

    for(size_t i = 0; i <= lenb; i++){
        btmp[i + needsign] = b[i];
    }

    if(!needsign){
        btmp[0] = '0';
    }

    const char *res = nsadd(a, btmp);
    free(btmp);
    return res;

}

const char* nsmul(const char *a, const char *b){
    int signa, signb;
    size_t lena, lenb, lenr, offseta, offsetb;
    const int addsize = 2;
    if(!CheckNumber(a, &signa, &lena) || !CheckNumber(b, &signb, &lenb)){
        return MakeZero();
    }

    lenr = lena + lenb + addsize;
    char *res = malloc(lenr);
    if(res == NULL){
        return MakeZero();
    }

    memset(res, 0, lenr);





    for (int ix = 0; ix < lena; ix++)
        for (int jx = 0; jx < lenb; jx++)
            res[ix + jx - 1] += (a[ix] - '0') * (b[jx] - '0');

    for (int ix = 0; ix < lenr - addsize + 1; ix++)
    {
        res[ix + 1] +=  res[ix] / 10 + '0';
        res[ix] %= 10;
        res[ix] += '0';
    }
    return res;

}
