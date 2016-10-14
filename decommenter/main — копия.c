#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

unsigned char *GetTextFromFile(char* fn){
    size_t c=1;
    int key;
    unsigned char *s,*s1;
    s = (char*) malloc((c)*sizeof(char));
    if (s==NULL) return NULL;
    s[c-1] = '\0';
    FILE *file;
    file = fopen(fn, "r");
    if(file==NULL) return NULL;
    while ((key = fgetc (file)) != EOF) {
        s[c-1] = key;
        c++;
        s1 = (char*)realloc(s, (c+1)*sizeof(char));
        if (s1==NULL) break;
        s = s1;
        s[c-1] = '\0';
    }
    s[c] = '\0';
    fclose(file);
    return s;
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
    if(c != '\0')
        putc(c, stdout);
    // strcat(

}
char IsCommentChar(char c){
    return(c == '/' || c == '*' || c == '\n');
}
char IsQuote(char c){
    return (c == 34 || c == 39);
}
char* DeComment(char* str){

}

int main()
{
    unsigned char *str;
    str = GetTextFromConsole();

    if(str == NULL)
    {
        printf("\nClear RAM\n");
        return 1;
    }

    char p='\0',c='\0';
    int i=0;
    char comment1 = 0,comment2 = 0;
    char quote1 = 0, quote2 = 0;

    while(str[i]!='\0')
    {
        c = str[i];
        if ((comment1 || comment2) && !IsCommentChar(c)) {
            p = c;
            i++;
            continue;
        }
        if (!(quote1 || quote2) || IsQuote(c)){
            switch(c){
            case 34://"
                quote1 = !quote1;
                break;
            case 39://'
                quote2 = !quote2;

                break;
            case '\n':
                if(comment2){
                    i++;
                    continue;
                }

                comment1 = 0;
                break;
            case '/':
                if(p == '*'){
                    comment2 = 0;
                    p = 0;
                    i++;
                    continue;
                }
                else if(p == '/'){
                    comment1 = 1;
                    p = 0;
                    i++;
                    continue;
                }
                else {
                    break;
                }

                break;
            case '*':

                if(p == '/')
                {
                    comment2 = 1;
                    p=0;
                    break;
                }
                else if(!comment1 && !comment2)
                {
                    break;
                }
                else
                {
                    p = c;
                    i++;
                    continue;
                }

                break;
            }
        }
        AddToChar(p);
        p = c;
        i++;
    }
    if(!comment1)
    {
        AddToChar(p);
    }
    if(comment2){
        printf("\nError\n");
    }
    return 0;
}
