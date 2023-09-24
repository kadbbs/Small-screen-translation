
#include <iostream>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <fcntl.h>
#include <string>
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <uuid/uuid.h>
#include <time.h>
#include <curl/curl.h>
using namespace std;


string sha256(const string str)
{
    char buf[2];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::string NewString = "";
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(buf,"%02x",hash[i]);
        NewString = NewString + buf;
    }
        return NewString;
}


int get_zh(char *ch,int size_ch,char *buf,int size_buf){

    string translatorfrom="auto";
    string translatorto="auto";
    
    string youdao_url="https://openapi.youdao.com/api";

    string app_id="127c6fecbcab8a42";

    string key="Nv7Ri1bIdEFV10rzLumALaUkQqEH7GUH";
    uuid_t uu;
    uuid_generate(uu);
    char uuid_str[37];
    string uu_str=uuid_str;
    uuid_unparse_lower(uu,uuid_str);
    string input="apple";
    time_t now=time(NULL);
    string time=to_string(now);
    string signType="v3";
    string sha_string=app_id+input+uu_str+time+key;
    //app_id,input,salt+curtion+key
    string  sign=sha256(sha_string);





    CURL *curl;
    CURLcode res;
    curl=curl_easy_init();


    string translate_url=youdao_url+"app_id"+app_id+"&q="+q+"&from"+from+"&to="+to+"&salt"+salt+"&sign"+sign;

    if(curl){

        curl_easy_setopt(curl, CURLOPT_URL, translate_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    

        






















    return 1;

}
