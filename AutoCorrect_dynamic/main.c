#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
unsigned char *GetTextFromConsole(){
    size_t c=1;
    int key;
    unsigned char *s,*s1;
    s = (char*) malloc((c)*sizeof(char));
    if (s==NULL) return NULL;
    s[c-1]='\0';
    while ((key = getchar ()) != 10) {
        s[c-1]=key;
        c++;
        s1 = (char*)realloc(s, (c+1)*sizeof(char));
        if (s1==NULL) break;
        s=s1;
        s[c-1]='\0';
    }
    s[c]='\0';
    return s;
}
void AddToChar(char c){
    if(c!='\0')
        putc(c, stdout);

}

int main()
{
    unsigned char *str;
    str = GetTextFromConsole();

    if(str==NULL)
    {
        printf("\nClear RAM\n");
        return 0;
    }
    unsigned char p='\0',c='\0';
    int i=0,Up=1,First=1;

    while(str[i]!='\0')
    {   c=str[i];
        if(First==1 && !IsAlphabetic(c)) continue;
        First=0;
        switch(c){
        case '.':
        case '?':
        case '!':
            if(p!=' ')
                AddToChar(p);
            AddToChar(c);
            p=' ';
            Up=1;
            break;
        case ',':
        case ':':
        case ';':
            if(p!=' ')
                AddToChar(p);
            AddToChar(c);
            p=' ';
            break;
        case '-':

            AddToChar(p);
            if(p!=' ')
                AddToChar(' ');
            AddToChar(c);
            AddToChar(' ');
            p=' ';
            break;
        case ' ':
            if(p==' ')
                break;
        default:
            if(Up==1){

                AddToChar(p);
                if(c!=' '){
                    Up=0;
                    AddToChar(ToUpper(c));
                    p='\0';
                }
            } else{
                AddToChar(ToLower(p));
                p=c;
            }
            break;
        }
        i++;
    }
    AddToChar(ToLower(p));
    if(!IsAlphabetic(c) && c!=',' && c!=';' && c!=':' && c!='-')
        AddToChar(ToLower(c));
    else
 AddToChar('.');
    return 0;
}
