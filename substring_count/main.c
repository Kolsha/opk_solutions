#include <stdio.h>
#include <string.h>
#include <assert.h>
int substring_count(char *string, char *substring)
{
    int lenstr = strlen(string);
    int lensubstr = strlen(substring);
    if (lenstr < 1 || lensubstr < 1 || lensubstr > lenstr){
        return 0;
    }
    int count = 0;
    for(int i=0; i <= lenstr-lensubstr; i++){
        if (string[i] == substring[0]){
            char match = 1;
            for(int c=0; c < lensubstr; c++)
            {
                match = string[i+c] == substring[c];
                if(!match) break;
            }
            if(match)
                count++;
        }
    }
    return count;
}


int main()
{
    assert(substring_count("abcabc", "ab") == 2);
    assert(substring_count("abcabcd", "d") == 1);
    assert(substring_count("abcabcd", "q") == 0);
    assert(substring_count("aaaaaa", "aa") == 5);
    assert(substring_count("aaaaaa", "aaaaaaa") == 0);
    return 0;
}
