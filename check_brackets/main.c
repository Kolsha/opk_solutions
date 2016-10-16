#include <stdio.h>
#include <assert.h>

char *GetTextFromConsole(){
    size_t c = 1;
    int key;
    char *s, *s1;
    s = (char*) malloc((c) * sizeof(char));
    if (s == NULL) {
        return NULL;
    }
    s[c-1] = '\0';
    while ((key = getchar ()) != 10) {
        s[c-1] = key;
        c++;
        s1 = (char*) realloc(s, (c + 1) * sizeof(char));
        if (s1 == NULL){
            break;
        }
        s = s1;
        s[c-1] = '\0';
    }
    s[c] = '\0';
    return s;
}

int strlen(const char *str){
    int len = 0;
    while(str[len] != 0)
    {
        len++;
    }
    return len;
}
unsigned char InverseBracket(unsigned char c){
    switch(c){
    case '(':
        return ')';
        break;
    case '{':
        return '}';
        break;
    case '[':
        return ']';
        break;
    case '<':
        return '>';
        break;
    case ')':
        return '(';
        break;
    case '}':
        return '{';
        break;
    case ']':
        return '[';
        break;
    case '>':
        return '<';
        break;
    default:
        return 0;
    }
}

int IsClosedBracket(unsigned char c){
    return (c==')' ||
            c=='}' ||
            c==']' ||
            c=='>');
}

int IsOpenedBracket(unsigned char c){
    return (c=='(' ||
            c=='{' ||
            c=='[' ||
            c=='<');
}

int IsBracket(unsigned char c){
    return IsOpenedBracket(c) || IsClosedBracket(c);
}


int check_brackets(char *expression)
{
    int i=0;
    int count=0;
    while(expression[i] != '\0')
    {

        unsigned char c = expression[i];
        i++;
        if(IsOpenedBracket(c)) {
            count++;
            continue;
        }
        if(!IsClosedBracket(c)){
            continue;
        }
        count--;
        unsigned char ic = InverseBracket(c);
        unsigned char state = 0;

        for(int down  = i - 2; down >= 0; down--){
            if(IsBracket(expression[down]) && expression[down] != ic)
            {
                state = 0;
                break;
            }
            else if(expression[down] == ic){
                state = 1;
                expression[down] = '-';
                expression[i-2] = '-';
                break;
            }
        }
        if(state==0){
            return 0;
        }

    }
    return ( count == 0);
}
void Tests()
{
    unsigned char FirstTest[] =     "()";
    unsigned char SecondTest[] =     "Empty test";
    unsigned char FirdTest[] =     "< Is { failed ( a [ test (O_o) > } ) ] ";


    assert(check_brackets(FirstTest)==1);

    assert(check_brackets(SecondTest)==1);

    assert(check_brackets(FirdTest)==1);


}
int main(int argc, char *argv[])
{
    Tests();
    unsigned char *str;
    printf("\nTry: ");
    str = GetTextFromConsole();

    if(str==NULL)
    {
        printf("\nClear RAM\n");
        return 1;
    }
    printf("Valid: %d",check_brackets(str));
    return 0;
}
