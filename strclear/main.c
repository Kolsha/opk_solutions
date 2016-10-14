#include <stdio.h>
#include <ctype.h>
int strlen(const char *str){
    int len = 0;
    while(str[len] != 0)
    {
        len++;
    }
    return len;
}
void delsym_and_past_space(char *str, size_t sym_pos, size_t str_len){
    if(sym_pos > 0)
    {
        str[sym_pos-1] = ' ';
    }
    for(int i = sym_pos; i <= str_len; i++)
        str[i] = str[i+1];
}
void delsym(char *str, size_t sym_pos, size_t str_len){
    for(int i = sym_pos; i <  str_len; i++)
        str[i] = str[i+1];
}
unsigned char *GetTextFromConsole(){
    size_t c=1;
    int key;
    unsigned char *s,*s1;
    s = (char*) malloc((c)*sizeof(char));
    if (s==NULL) return NULL;
    s[c-1] = '\0';
    while ((key = getchar ()) != 10) {
        s[c-1] = key;
        c++;
        s1 = (char*)realloc(s, (c+1)*sizeof(char));
        if (s1==NULL) break;
        s = s1;
        s[c-1] = '\0';
    }
    s[c] = '\0';
    return s;
}
void strclear(char *string)
{
    int i = 0;
    int len = strlen(string);
    int prev = 0;
    while(string[i]!='\0' && len > 0){
        int c=string[i];
        if(prev != 0 && isspace(string[i]))
        {
            //delsym(string,i,len);
            delsym_and_past_space(string,i,len);
            len--;
            i--;
        }
        prev = isspace(string[i]);
        i++;
    }
}
void Tests()
{
    char TabTest[] =     "               Tab             test            ";
    char SpaceTest[] =     "                Space             test                 ";
    char NTest[] =     "\n\n\n N\n\n\ntest\n\n\n\n\n\n\n\n\n";

    printf("\nOriginal: %s",TabTest);
    strclear(TabTest);
    printf("\nFixed: %s",TabTest);


    printf("\nOriginal: %s",SpaceTest);
    strclear(SpaceTest);
    printf("\nFixed: %s",SpaceTest);

    printf("\nOriginal: %s",NTest);
    strclear(NTest);
    printf("\nFixed: %s",NTest);
}

int main(int argc, char *argv[])
{
    Tests();
    printf("\nTry:");
    unsigned char *str;
    str = GetTextFromConsole();

    if(str==NULL)
    {
        printf("\nClear RAM\n");
        return 1;
    }
    strclear(str);
    printf("%s",str);
    return 0;
}
