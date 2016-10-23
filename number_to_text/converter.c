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

char *dig2str (unsigned long long p_summa, int p_sex, char *p_one, char *p_four, char *p_many)
{
    if(p_summa == 0UL){
        return strconcat("ноль ", p_many);
    }

    a_power[0].sex  = p_sex;
    a_power[0].one  = p_one;
    a_power[0].four = p_four;
    a_power[0].many = p_many;

    char *res = malloc(1);
    char *tmp = NULL;
    if(res == NULL){
        return NULL;
    }
    res[0] = '\0';

    if(p_summa <  0UL) {
        tmp = strconcat(res , "минус ");
        free(res);
        if(tmp == NULL){
            return NULL;
        }
        res = tmp;
        p_summa *= -1;

    }
    unsigned long long  divisor = 1;
    for(size_t i = 0; i < DG_POWER; i++)
    {
        divisor *= 1000UL;
    }

    for(int i = DG_POWER - 1; i >= 0; i--){
        divisor /= 1000;
        int mny = (int)(p_summa / divisor);
        p_summa %= divisor;

        char *str = malloc(1);
        if(str == NULL){
            return NULL;
        }
        str[0] = '\0';

        if(mny == 0){
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
            if(mny >= 100){
                tmp = strconcat(str, digit[mny / 100].hun);
                free(str);
                if(tmp == NULL){
                    free(res);
                    return NULL;
                }
                str = tmp;
                mny %=100;
            }
            if(mny >= 20 ){
                tmp = strconcat(str, digit[mny / 10].dec);
                free(str);
                if(tmp == NULL){
                    free(res);
                    return NULL;
                }
                str = tmp;
                mny %=10;
            }
            if(mny >= 10 ){
                tmp = strconcat(str, digit[mny - 10].two);
                free(str);
                if(tmp == NULL){
                    free(res);
                    return NULL;
                }
                str = tmp;
            }else if(mny >= 1  ){
                tmp = strconcat(str, digit[mny].one[a_power[i].sex]);
                free(str);
                if(tmp == NULL){
                    free(res);
                    return NULL;
                }
                str = tmp;
            }
            switch(mny){
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

