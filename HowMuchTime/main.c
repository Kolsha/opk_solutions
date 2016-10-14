#include <stdio.h>

int HowMuchTime(double start,double end, int period, double percent){
    int days = 0;
    while(start < end){
        days += period;
        if(days % period == 0){
            start += percent * start / 100;
        }

    }
    return days;
}

int main(int argc, char *argv[])
{
    printf("%d", HowMuchTime(100,400,17,10));
    return 0;
}
