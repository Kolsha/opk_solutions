#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char *GetTextFromFile(char* fn){
    size_t c=1;
    int key;
    char *s,*s1;
    s = (char*) malloc((c)*sizeof(char));
    if (s==NULL) return NULL;
    s[c-1] = '\0';
    FILE *file;
    file = fopen(fn, "r");
    if(file==NULL) return NULL;
    while ((key = fgetc (file)) != EOF) {
        s[c-1] = key;
        c++;
        s1 = (char*)realloc(s, (c+1) * sizeof(char));
        if (s1==NULL) break;
        s = s1;
        s[c-1] = '\0';
    }
    s[c] = '\0';
    fclose(file);
    return s;
}

void AddToChar(char* in,char c){
    if(c == '\0' || in == NULL)
        return;
    int i;
    for(i = 0; in[i] != 0; i++){
    }
    in[i] = c;
}
char IsCommentChar(char c){
    return(c == '/' || c == '*' || c == '\n');
}
char IsQuote(char c){
    return (c == 34 || c == 39);
}
char* DeComment(char* str){
    if(str == NULL)
    {
        return NULL;
    }

    char p = '\0', c = '\0';
    char comment1 = 0, comment2 = 0;
    char quote1 = 0, quote2 = 0;

    int lenstr = strlen(str);
    int i = 0;

    char* res = malloc(sizeof(char) * lenstr+1);
    if(res == NULL) {
        return NULL;
    }
    memset(res, 0, sizeof(char) * lenstr+1);

    int count92 = 0;
    while(str[i]!='\0')
    {
        c = str[i];

        if ((comment1 || comment2) && !IsCommentChar(c)) {
            p = c;
            i++;
            continue;
        }
        if(quote1 || quote2){
            count92++;
        }
        else {
            count92 = 0;
        }
        if (!(quote1 || quote2) || IsQuote(c)){
            switch(c){
            case '"':
                if(count92 % 2 == 0)
                    quote1 = !quote1;
                break;
            case '\'':
                if(count92 % 2 == 0)
                    quote2 = !quote2;
                break;
            case '\n':
                if(comment2 || (p == c  && (lenstr-i-2) >= 0 && str[i-2] != '\\' )){
                    i++;
                    continue;
                }
                if (comment1){
                    p = 0;
                }
                comment1 = 0;
                break;
            case '/':
                if(p == '*' && (lenstr-i-2) >= 0 && str[i-2] != '/'){
                    comment2 = 0;
                    p = 0;
                    i++;
                    continue;
                }
                else if(p == '/'){
                    comment1 = !comment2;
                    p = 0;
                    i++;
                    continue;
                }
                else if(comment1 || comment2){
                    p = 0;
                    i++;
                    continue;
                }
                else{
                    break;
                }

                break;
            case '*':

                if(p == '/')
                {
                    comment2 = !comment1;
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
        AddToChar(res, p);
        p = c;
        i++;
    }
    if(!comment1)
    {
        AddToChar(res, p);
    }
    if(comment2){
        free(res);
        return NULL;
    }
    return res;
}

int main(int argc, char *argv[])
{

    if(argc != 3){
        printf("Usage:\ndecommenter file.c out.c");
        exit(1);
    }
    char *str;
    char *res;

    str = GetTextFromFile(argv[1]);
    res = DeComment(str);
    free(str);

    if(res == NULL){
        printf("Bad in file");
        exit(1);
    }
    int len_out = strlen(res);

    FILE *file;
    file = fopen(argv[2], "w");
    if(file==NULL) exit(1);

    for(int i = 0; i < len_out; i++)
    {
        fputc(res[i],file);
    }

    fclose(file);
    printf("Complete");
    free(res);
    return 0;
}
