#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <stdarg.h>

#include "myHttp.h"

#include <iconv.h>

#define CURL_ICONV_CODESET_OF_HOST "UTF-8"
#define CURL_ICONV_CODESET_FOR_UTF8 "ASCII"

static httpclient mHttp = {30, "text/plain; q=0.5, text/html; charset=ASCII;", "MaffiaBot", "", 0, -1};

static char *ERRORS[] = {"unknown", "init", "socket", "memory", "bad data", "network"};

struct MemoryStruct {
    char *memory;
    size_t size;
};


static struct MemoryStruct chunk = {NULL, 0};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *memory = realloc(mem->memory, mem->size + realsize + 1);

    if(memory == NULL) {
        mHttp.error_num = 3;
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }
    mem->memory = memory;

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}


int http_init()
{
    mHttp.state = 1;
    chunk.memory = NULL;
    chunk.size = 0;

    return (curl_global_init(CURL_GLOBAL_DEFAULT) == CURLE_OK);
}

void http_free(){
    curl_global_cleanup();
}

char *http_last_error(){

    return ((mHttp.error_num) ? ERRORS[mHttp.error_num] : NULL);
}

static CURL *my_curl_init(){

    CURL *curl;

    curl = curl_easy_init();
    if(curl) {
        chunk.memory = malloc(1);
        chunk.size = 0;
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, mHttp.user_agent);
        //curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
        //curl_easy_setopt(curl, CURLOPT_CONV_FROM_UTF8_FUNCTION, NULL);
        return curl;
    }
    return NULL;
}

static char *http_request(char *url, char *post){
    mHttp.error_num = 0;
    if(!mHttp.state && !http_init()){
        mHttp.error_num = 1;
        return NULL;
    }

    if(url == NULL){
        mHttp.error_num = 4;
        return NULL;
    }

    CURL *curl;
    CURLcode res;



    curl = my_curl_init();
    if(curl) {

        curl_easy_setopt(curl, CURLOPT_URL, url);
        if(post != NULL && strlen(post) > 0){
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
        }
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if(res != CURLE_OK)
        {
            if(mHttp.error_num == 0)
            {
                mHttp.error_num = 2;
            }
            free(chunk.memory);
            return NULL;
        }

        return chunk.memory;

    }
    mHttp.error_num = 2;
    return NULL;
}

char *http_get(char *url){
    return http_request(url, NULL);
}

char *http_post(char *url, char *data){
    return http_request(url, data);
}


