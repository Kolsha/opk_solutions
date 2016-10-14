#include "rational.h"
#include <math.h>

static int gcd(int p, int q){
    return (q == 0) ? p : gcd(q, p % q);
}

static int power(int p, int q){
    int res = 1;
    if (q < 0)
    {
        return res;
    }
    while(q){
        if(q & 1){
            res *= p;
        }
        p *= p;
        q >>= 1;
    }
    return res;
}

static void normalize(Rational *rat) {
    int g = gcd(rat->numer, rat->denom);
    rat->numer /= g;
    rat->denom /= g;
    if(rat->numer == 0 || rat->denom == 0){
        rat->denom = 0;
        rat->numer = 0;
    }
    if(rat->denom < 0){
        rat->numer *= -1;
        rat->denom *= -1;
    }
}

static void rat_to_one_denom(Rational *a, Rational *b){
    if(a->denom == b->denom){
        return;
    }
    a->numer *= b->denom;
    b->numer *= a->denom;
    a->denom *= b->denom;
    b->denom = a->denom;
}

static void rat_swap(Rational *a){
    int t = a->denom;
    a->denom = a->numer;
    a->numer = t;
}

void rat_mul(Rational *result, Rational *a, Rational *b) {
    result->numer = a->numer * b->numer;
    result->denom = a->denom * b->denom;
    normalize(result);
}

void rat_power(Rational *result, Rational *r, int pow){
    if(pow < 0){
        pow *= -1;
        result->denom = power(r->numer, pow);
        result->numer = power(r->denom, pow);
    }else{
        result->denom = power(r->denom, pow);
        result->numer = power(r->numer, pow);
    }
}

void rat_create(Rational *result, int a, int b){
    /*struct Rational *el = (struct Rational *) malloc(sizeof(struct Rational));
    if(el == NULL){
        return ;
    }*/
    if(result == NULL){
        return ;
    }

    result->denom = b;
    result->numer = a;
    normalize(result);
}

void rat_add(Rational *result, Rational *a, Rational *b){
    normalize(a);
    normalize(b);
    Rational at, bt;
    at.denom = a->denom;
    at.numer = a->numer;

    bt.denom = b->denom;
    bt.numer = b->numer;

    rat_to_one_denom(&at,&bt);

    result->denom = at.denom;
    result->numer = at.numer + bt.numer;

    normalize(result);
}

void rat_print(Rational *a, FILE *fp){
    int m = abs(a->denom);
    if(abs(a->numer) > m){
        m = abs(a->numer);
    }

    if(a->numer < 0){
        fprintf(fp, "  ");
    }
    fprintf(fp, "%d\n", abs(a->numer));
    if(a->numer < 0){
        fprintf(fp, "- ");
    }
    while(m > 0){
        m /= 10;
        fprintf(fp, "-");
    }
    fprintf(fp, "\n");

    if(a->numer < 0){
        fprintf(fp, "  ");
    }
    fprintf(fp, "%d\n", abs(a->denom));
}

int rat_compare(Rational *a, Rational *b){

    Rational at, bt;
    at.denom = a->denom;
    at.numer = a->numer;

    bt.denom = b->denom;
    bt.numer = b->numer;


    normalize(&at);
    normalize(&bt);

    rat_to_one_denom(&at,&bt);

    if(at.numer == bt.numer){
        return 0;
    }else if(at.numer < bt.numer){
        return -1;
    }else{
        return 1;
    }
}

void rat_sub(Rational *result, Rational *a, Rational *b){
    b->numer *= -1;
    rat_add(result, a, b);
    b->numer *= -1;
}

void rat_div(Rational *result, Rational *a, Rational *b){
    rat_swap(b);
    rat_mul(result, a, b);
    rat_swap(b);
}

int rat_to_i(Rational *rat){
    return floor((double)rat->numer / (double)rat->denom);
}

double rat_to_f(Rational *a){
    return (double)a->numer / (double)a->denom;
}
