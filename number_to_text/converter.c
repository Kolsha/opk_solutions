#include "converter.h"

static struct s_POWER {
    int sex;
    char *one;
    char *four;
    char *many;
} a_power[] = {
{0,NULL          ,NULL           ,NULL            },
{1,"тысяча "     ,"тысячи "      ,"тысяч "        },
{0,"миллион "    ,"миллиона "    ,"миллионов "    },
{0,"миллиард "   ,"миллиарда "   ,"миллиардов "   },
{0,"триллион "   ,"триллиона "   ,"триллионов "   },
{0,"квадриллион ","квадриллиона ","квадриллионов "},
{0,"квинтиллион ","квинтиллиона ","квинтиллионов "}
};

static struct s_UNIT {
    char *one[2];
    char *two;
    char *dec;
    char *hun;
} digit[10] = {
{{""       ,""       },"десять "      ,""            ,""          },
{{"один "  ,"одна "  },"одиннадцать " ,"десять "     ,"сто "      },
{{"два "   ,"две "   },"двенадцать "  ,"двадцать "   ,"двести "   },
{{"три "   ,"три "   },"тринадцать "  ,"тридцать "   ,"триста "   },
{{"четыре ","четыре "},"четырнадцать ","сорок "      ,"четыреста "},
{{"пять "  ,"пять "  },"пятнадцать "  ,"пятьдесят "  ,"пятьсот "  },
{{"шесть " ,"шесть " },"шестнадцать " ,"шестьдесят " ,"шестьсот " },
{{"семь "  ,"семь "  },"семнадцать "  ,"семьдесят "  ,"семьсот "  },
{{"восемь ","восемь "},"восемнадцать ","восемьдесят ","восемьсот "},
{{"девять ","девять "},"девятнадцать ","девяносто "  ,"девятьсот "}
};


int read_value(const char *request, const char *fmt, void *value) {
    for (;;) {
        if (request != NULL) {
            printf("%s: ", request);
        }
        int res = scanf(fmt, value);
        if (res == EOF) {
            // end of input
            return 0;
        }
        for (;;) {
            int ch = getchar();
            if (ch == EOF || ch == '\n') {
                break;
            }
        }
        if (res == 1) {
            return 1;
        }
    }
}

static char *strconcat(char *s1, char *s2){
    size_t ls1 = 0;
    size_t ls2 = 0;
    if(s1 != NULL)
    {
        ls1 = strlen(s1);

    }
    if(s2 != NULL){
        ls2 = strlen(s2);
    }

    char *res = malloc(ls1 + ls2 + 1);
    if(res == NULL){
        return NULL;
    }
    res[0] = '\0';
    if(s1 != NULL)
    {
        strcat(res, s1);
    }
    if(s2 != NULL)
    {
        strcat(res, s2);
    }

    return res;
}

char *dig2str (unsigned long long sum, int sex, char *one, char *four, char *many)
{
    if(sum == 0UL){
        return strconcat("ноль ", many);
    }

    a_power[0].sex  = sex;
    a_power[0].one  = one;
    a_power[0].four = four;
    a_power[0].many = many;

    char *res = malloc(1);
    char *tmp = NULL;
    if(res == NULL){
        return NULL;
    }
    res[0] = '\0';

    /*if(sum <  0UL) {
        tmp = strconcat(res , "минус ");
        free(res);
        if(tmp == NULL){
            return NULL;
        }
        res = tmp;
        sum *= -1;

    }*/

    unsigned long long  divisor = 1;
    for(size_t i = 0; i < DG_POWER; i++)
    {
        divisor *= 1000UL;
    }

    for(int i = DG_POWER - 1; i >= 0; i--){
        divisor /= 1000;
        int rest = (int)(sum / divisor);
        sum %= divisor;

        char *str = malloc(1);
        if(str == NULL){
            return NULL;
        }
        str[0] = '\0';

        if(rest == 0){
            if(i > 0)
            {
                continue;
            }
            tmp = strconcat(str, a_power[i].one);
            free(str);
            if(tmp == NULL){
                free(res);
                return NULL;
            }
            str = tmp;
        }else {
            if(rest >= 100){
                tmp = strconcat(str, digit[rest / 100].hun);
                free(str);
                if(tmp == NULL){
                    free(res);
                    return NULL;
                }
                str = tmp;
                rest %=100;
            }
            if(rest >= 20 ){
                tmp = strconcat(str, digit[rest / 10].dec);
                free(str);
                if(tmp == NULL){
                    free(res);
                    return NULL;
                }
                str = tmp;
                rest %=10;
            }
            if(rest >= 10 ){
                tmp = strconcat(str, digit[rest - 10].two);
                free(str);
                if(tmp == NULL){
                    free(res);
                    return NULL;
                }
                str = tmp;
            }else if(rest >= 1  ){
                tmp = strconcat(str, digit[rest].one[a_power[i].sex]);
                free(str);
                if(tmp == NULL){
                    free(res);
                    return NULL;
                }
                str = tmp;
            }
            switch(rest){
            case 1:
                tmp = strconcat(str, a_power[i].one);
                free(str);
                if(tmp == NULL){
                    free(res);
                    return NULL;
                }
                str = tmp;
                break;
            case 2:
            case 3:
            case 4:
                tmp = strconcat(str, a_power[i].four);
                free(str);
                if(tmp == NULL){
                    free(res);
                    return NULL;
                }
                str = tmp;
                break;
            default:
                tmp = strconcat(str, a_power[i].many);
                free(str);
                if(tmp == NULL){
                    free(res);
                    return NULL;
                }
                str = tmp;
                break;
            }
        }
        tmp = strconcat(res , str);
        free(res);
        if(str != NULL){
            free(str);
        }
        if(tmp == NULL){
            return NULL;
        }
        res = tmp;
    }
    return res;
}

