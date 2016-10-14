#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define linelen 21

char *weekdays = "Su Mo Tu We Th Fr Sa";
char *monthname[] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};
int CountOfDays(const struct tm * timeinfo){
    int DayOfMonth[]={31,28,31,30,31,30,31,31,30,31,30,31};
    if(timeinfo->tm_mon == 1){
        int year = timeinfo->tm_year + 1900;
        return (year%400 == 0 || year%4 == 0) ? 29 : 28;
    }
    return DayOfMonth[timeinfo->tm_mon];
}

char **cal_month(unsigned int year, unsigned int month){
    if(year < 1900 || year > 9999 || month < 1 || month > 12)
        return NULL;
    time_t rawtime;
    struct tm * timeinfo;
    char** result = malloc(sizeof(char*)*9);
    if(result == NULL){
        return NULL;
    }
    result[7] = NULL;
    result[8] = NULL;
    for(int i = 0; i < 7; i++)
    {
        result[i] = malloc(sizeof(char)*linelen);
        memset(result[i], ' ', sizeof(char)*linelen);
        result[i][linelen-1] = 0;
    }

    time( &rawtime );
    timeinfo = localtime( &rawtime );
    timeinfo->tm_year = year - 1900;
    timeinfo->tm_mon = month - 1;
    timeinfo->tm_mday = 1;
    mktime( timeinfo );
    int days = CountOfDays(timeinfo);
    int curline = 0;
    sprintf(result[curline], "%10s %d", monthname[timeinfo->tm_mon],year);
    curline++;
    result[curline] = strdup(weekdays);

    curline++;
    int linepos=0;
    //char* debug = result[curline];
    for(int i = 0; i < timeinfo->tm_wday; i++)
    {
        result[curline][linepos++] = ' ';
        result[curline][linepos++] = ' ';
        result[curline][linepos++] = ' ';
    }
    for(int i = 1; i <= days; i++)
    {
        timeinfo->tm_mday = i;
        mktime(timeinfo);
        char number[5] = {0};
        itoa(i,number,10);
        if(i<10){
            result[curline][linepos++] = ' ';
            result[curline][linepos++] = number[0];
        }
        else{
            result[curline][linepos++] = number[0];
            result[curline][linepos++] = number[1];
        }

        if(timeinfo->tm_wday == 6)
        {
            curline++;
            linepos=0;
            if(curline==7 && i!=days){
                result[curline] = malloc(sizeof(char)*linelen);
                memset(result[curline],' ',sizeof(char)*linelen);
                result[curline][linelen-1] = 0;
                result[curline+1] = NULL;
            }
        }
        else
        {
            result[curline][linepos++] = ' ';
        }
    }
    if(result[7] == NULL){
        free(result[8]);
    }
    return result;



}


char **cal_year(unsigned int year){
    if(year < 1900 || year > 9999)
        return NULL;
    char** result = malloc(sizeof(char*)*35);
    if(result == NULL){
        return NULL;
    }
    result[34] = NULL;
    for(int i = 0; i < 34; i++)
    {
        result[i] = malloc(sizeof(char)*(linelen*3 + 4));
        memset(result[i],0,sizeof(char)*(linelen*3 + 4));
    }

    sprintf(result[0], "%30d", year);
    memset(result[1], 32, sizeof(char)*(linelen*3 + 3));
    int offset = 2;
    for(int i = 1; i <= 12; i++){
        char** cal;
        cal = cal_month(year,i);
        int j = 0;
        while(2<3){
            if(cal[j] == NULL)
                break;
            if(result[offset+j][0] != 0){
                strcat(result[offset+j],"  ");
            }
            if(j == 0){
                char monthnamebuff[linelen];
                sprintf(monthnamebuff,"%20s",monthname[i-1]);
                char state=0;
                for(int m=0; m < linelen; m++){
                    if(state == 1 || monthnamebuff[i]==0){
                        state = 1;
                        monthnamebuff[m]=' ';
                    }
                }
                monthnamebuff[linelen-1] = 0;
                strcat(result[offset+j], monthnamebuff);
            }
            else
            {
                strcat(result[offset+j],cal[j]);
            }
            j++;
        }

        if(i % 3 == 0) {
            for(int j = offset; j < 34; j++)
            {
                if(result[j][0] == 0){
                    offset = j;
                    break;
                }
            }
        }
    }
    return result;
}

void cal_free_result(char **result){
    for(int i = 0 ; result[i] != NULL ; i++){
        free(result[i]);
    }
    free(result);
}



int main ()
{


    //int year, month;

    char** cal;

    //printf("\nYear: ");
    //scanf("%d", &year);
    //printf("\nMoth: ");
    //scanf("%d", &month);
    cal = cal_year(2016);
    //cal = cal_month(2011,3);
    if(cal == NULL ) return 0;
    int i = 0;
    while(2<3){

        if(cal[i] == NULL) break;
        printf("%s\n",cal[i]);
        i++;
    }
    cal_free_result(cal);

    return 0;
}
