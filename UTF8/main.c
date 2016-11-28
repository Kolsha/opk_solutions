#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *codepoint_to_utf8b(unsigned cp){
/*
 * http://stackoverflow.com/questions/6240055/manually-converting-unicode-codepoints-into-utf-8-and-utf-16
 */
    if(cp == 0){
        return strdup("0");
    }
    int i;
    for(i = 32; i >= 0;i--){
        if( ((cp >> i) & 1) == 1)
        {
            break;
        }
    }
    unsigned res = 0;

    if(i <= 7){
        res = cp;
    }else if(i <= 11){

    }else if(i <= 16){

    }else if(i <= 21){

    }else{
        return NULL;
    }

}

char *utf16_to_codepoint(char *utf16){

    if(utf16 == NULL){
        return NULL;
    }
    char *hex = strdup("D83D");
    unsigned dec = 0;

    if(hex == NULL){
        return NULL;
    }

    dec =  strtol(hex, &hex, 16);
    if(dec < 0xD800 || dec > 0xDFFF){
        return hex;
    }else if(dec >= 0xDC00){
        return NULL;
    }else{
        char *nex = strdup("DCF7");
        unsigned nec = strtol(nex, &nex, 16);
        unsigned res = ((dec & 0x3FF)  << 10);

        if ((nec < 0xDC00) || (nec > 0xDFFF)){
            return NULL;
        }else
        {
            res = res | ((nec & 0x3FF));
            res = (res + 0x10000);
            printf("%X\n", res);
            //return convert to hex
        }

    }


}




int main(void)
{
     utf16_to_codepoint("FFFF");//printf("%s\n",);
    return 0;
}

