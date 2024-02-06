#include <stdio.h>
#include <curl/curl.h>

int main(int argc, char** argv)
{
    
    CURL *curl = curl_easy_init();
    if(curl) {
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_URL, "https://wttr.in/Moscow?format=j1");
        res = curl_easy_perform(curl);
        if (res == CURLE_OK)    {
            printf("Всё ОК!!!\n");        
        }
        curl_easy_cleanup(curl);
    }
    return 0;
}