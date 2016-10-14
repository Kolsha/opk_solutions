#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#define StrSize 1000
void AddToChar(char* CArr,char c){
    int i;
    for ( i = 0; i < StrSize; i++ )
    {
        if ( CArr[i] == '\0' )
        {
            CArr[i] =  c;
            return;
        }
    }
}
char ToUpper(unsigned char c){
    if(c==241)return 240;
    if(c>=160 && c<=175)
        return (c-32);
    if(c>=224 && c<=239)
        return (c-80);
    return c;
}
char ToLower(unsigned char c){
    if(c==240)return 241;
    if(c>=128 && c<144)
        return (c+32);
    if(c>=144 && c<=159)
        return (c+80);
    return c;
}
int IsAlphabetic(unsigned char c){
    if(c==241||c==240||c==34) return 1;
    if(c>=128 && c<=175) return 1;
    if(c>=224 && c<=239) return 1;
    if(c>=48 && c<=57) return 1;
    return 0;
}

int main()
{
    unsigned char str[StrSize]={'\0'},Fixed[StrSize]={'\0'},p='\0',c='\0';
    int i,Up=1,First=1;
    fgets( str, StrSize, stdin );
    for ( i = 0; i < StrSize; i++ )
    {   c=str[i];
        if(First==1 && !IsAlphabetic(c)) continue;
        First=0;
        switch(c){
        case '\n':
            str[i]='\0';
            AddToChar(Fixed,ToLower(p));
            if(IsAlphabetic(p)) AddToChar(Fixed,'.');
            goto PrintScreen;
            break;
        case '.':
        case '?':
        case '!':
            if(p!=' ')
                AddToChar(Fixed,ToLower(p));
            AddToChar(Fixed,c);
            p=' ';
            Up=1;
            break;
        case ',':
        case ':':
        case ';':
            if(p!=' ')
                AddToChar(Fixed,p);
            AddToChar(Fixed,c);
            p=' ';
            break;
        case '-':

            AddToChar(Fixed,p);
            if(p!=' ')
                AddToChar(Fixed,' ');
            AddToChar(Fixed,c);
            AddToChar(Fixed,' ');
            p=' ';
            break;
        case ' ':
            if(p==' ')
                break;
        default:
            if(Up==1){

                AddToChar(Fixed,ToLower(p));
                if(c!=' '){
                    Up=0;
                    AddToChar(Fixed,ToUpper(c));
                    p='\0';
                }
            } else{
                AddToChar(Fixed,ToLower(p));
                p=c;
            }


            /*if((str[i]>=128 && str[i]<=159)|| str[i]==240)
              AddToChar(Fixed,ToLower(p)); //printf ("Big: %d %s", ToLower(str[i],  "\n");

            if((str[i]>=160 && str[i]<=175)||(str[i]>=224 && str[i]<=239)|| str[i]==241)
                //printf ("Small: %d %s", str[i],  "\n");
             AddToChar(Fixed,ToUpper(p));*/
            break;
        }

    }


PrintScreen: {

        printf( "%s", Fixed );
    }
    // printf( "%s", str );
    return 0;
}
