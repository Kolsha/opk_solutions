#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};


char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length) {

    *output_length = 4 * ((input_length + 2) / 3)+1;

    char *encoded_data = malloc(*output_length);
    if (encoded_data == NULL)
        return NULL;

    for (int i = 0, j = 0; i < input_length;) {

        int octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        int octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        int octet_c = i < input_length ? (unsigned char)data[i++] : 0;

        int triple = (octet_a << 2 * 8)
                + (octet_b << 1 * 8)
                + (octet_c << 0 * 8);

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];

    }

    for (int i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 2 - i] = '=';
    encoded_data[*output_length-1] = 0;
    return encoded_data;
}


unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length) {

    if (decoding_table == NULL) build_decoding_table();

    // if (input_length % 4 != 0) return NULL;

    *output_length = (input_length / 4 * 3)+1;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    unsigned char *decoded_data = malloc(*output_length);
    if (decoded_data == NULL) return NULL;
    int error = 0;
    for (int i = 0, j = 0; i < input_length;) {
        /*if(data[i] == '\n') {
            i++;
            input_length--;
        }
        if(decoding_table[data[i]]>63){
            error = 1;
            break;
        }
        int sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        if(data[i] == '\n') {
            i++;
            input_length--;
        }
        if(decoding_table[data[i]]>63){
            error = 1;
            break;
        }
        int sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        if(data[i] == '\n') {
            i++;
            input_length--;
        }
        if(decoding_table[data[i]]>63){
            error = 1;
            break;
        }
        int sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        if(data[i] == '\n') {
            i++;
            input_length--;
        }
        if(decoding_table[data[i]]>63){
            error = 1;
            break;
        }
        int sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];

        int triple_ = (sextet_a << 3 * 6)
                + (sextet_b << 2 * 6)
                + (sextet_c << 1 * 6)
                + (sextet_d << 0 * 6);
*/
        int sextets[4];
        for(int c = 0; c < 4; c++){
            if(data[i] == '\n') {
                i++;
                input_length--;
            }
            if(decoding_table[data[i]]>63){
                free(decoded_data);
                return NULL;
            }
            sextets[c] = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        }
        int triple = (sextets[0] << 3 * 6)
                + (sextets[1] << 2 * 6)
                + (sextets[2] << 1 * 6)
                + (sextets[3] << 0 * 6);

        if (j < *output_length - 1) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length - 1) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length - 1) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }
    decoded_data[*output_length - 1]=0;
    if (input_length % 4 != 0 || error == 1){
        free(decoded_data);
        return NULL;
    }
    return decoded_data;
}


void build_decoding_table() {

    decoding_table = malloc(256);

    for (int i = 0; i < 64; i++)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}


void base64_cleanup() {
    free(decoding_table);
}

unsigned char *GetTextFromFile(char* fn){
    size_t c=1;
    int key;
    unsigned char *s,*s1;
    s = (char*) malloc((c)*sizeof(char));
    if (s==NULL) return NULL;
    s[c-1] = '\0';
    FILE *file;
    file = fopen(fn, "r");
    if(file == NULL) {
        free(s);
        return NULL;
    }
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

int strlen(const char *str){
    int len = 0;
    while(str[len] != 0)
    {
        len++;
    }
    return len;
}

int main(int argc, char *argv[])
{
    if(argc != 4){
        exit(1);
    }

    size_t len_in,len_out;
    char *str;
    str = GetTextFromFile(argv[2]);
    if(str == NULL){
        printf("Bad in file");
        return 1;
    }
    len_in = strlen(str);
    FILE *file;
    file = fopen(argv[3], "w");
    if(file==NULL)
        exit(1);

    if(argv[1][1]=='e'){
        str=base64_encode(str,len_in,&len_out);
        for(int i=0; i<=len_out; i++)
        {
            if(str[i]==0) break;
            fputc(str[i],file);
            if(i%72==0 && i!=0)
                fputc('\n',file);
        }
    }
    else if(argv[1][1]=='d'){
        str=base64_decode(str,len_in,&len_out);
        if (str == NULL) {
            printf("Bad in file");
            return 1;
        }
        for(int i=0; i<=len_out; i++)
        {
            if(str[i]==0) break;
            fputc(str[i],file);

        }
    }
    fclose(file);
    printf("Complete");
    free(str);
    base64_cleanup();
    return 0;
}
