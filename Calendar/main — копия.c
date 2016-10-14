#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define linelen 20

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
    char** result = malloc(sizeof(char*)*8);
    result[7] = NULL;
    for(int i = 0; i < 7; i++)
    {
        result[i] = malloc(sizeof(char)*linelen);
        result[i][linelen-1] = 0;
        //printf("%s\n",result[i]);
    }
    ///return NULL;
    time( &rawtime );
    timeinfo = localtime( &rawtime );
    timeinfo->tm_year = year - 1900;
    timeinfo->tm_mon = month - 1;
    timeinfo->tm_mday = 1;
    mktime( timeinfo );
    int days = CountOfDays(timeinfo);
    int curline=0;
    //printf("%10s %d\n",monthname[timeinfo->tm_mon],year);
    sprintf(result[curline],"%10s %d",monthname[timeinfo->tm_mon],year);
    curline++;
    result[curline]=strdup(weekdays);
    //printf(weekdays);
    //printf("\n");
    curline++;
    int linepos=0;
    for(int i = 0; i < timeinfo->tm_wday; i++)
    {
        result[curline][linepos++]=' ';
        result[curline][linepos++]=' ';
        result[curline][linepos++]=' ';
    }
    for(int i = 1; i <= days; i++)
    {
        timeinfo->tm_mday = i;
        mktime (timeinfo);
        char number[5] = {0};
        itoa(i,number,10);
        if(i<10){
            result[curline][linepos++]=' ';
            result[curline][linepos++]=number[0];
        }
        else{
            result[curline][linepos++]=number[0];
            result[curline][linepos++]=number[1];
        }
        result[curline][linepos++]=' ';
        //printf("%2d ",i);
        if(timeinfo->tm_wday == 6)
        {
            result[curline][linepos++]=0;
            curline++;
            linepos=0;
        }
    }
    for(int i = 0; i < (6 - timeinfo->tm_wday); i++)
    {
        result[curline][linepos++]=' ';
        result[curline][linepos++]=' ';
        if((i+1)!=(6 - timeinfo->tm_wday))
            result[curline][linepos++]=' ';
    }
    result[curline][linepos++]=0;

    return result;



}


char **cal_year(unsigned year);
void cal_free_result(char **result);



int main ()
{


    int year, month;

    char** cal;

    printf("\nYear: ");
    scanf("%d", &year);
    printf("\nMoth: ");
    scanf("%d", &month);
    cal = cal_month(year,month);
    if(cal == NULL ) return 0;
    for(int i = 0 ; cal[i] != NULL ; i++){
        printf("%s\n",cal[i]);
    }


    return 0;
}
