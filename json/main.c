#include "common.h"
#include "json_func.h"
#include <stdio.h>
#include <curl/curl.h>


int main(int argc, char** argv)
{
    int current_obj_idx = 0;
    int desc_idx = 0;
    json_object *root = NULL,  *current_day = NULL, *elem = NULL;
    char *data = "Moscow.json";
    /*CURL *curl = curl_easy_init();
    if(curl) {
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_URL, "https://wttr.in/Moscow?format=j1");
        res = curl_easy_perform(curl);
        if (res == CURLE_OK)    {
            printf("Всё ОК!!!\n");        
        }
        curl_easy_cleanup(curl);
    }*/
    //json_object * jobj = json_tokener_parse(string);     
    root  = json_object_from_file(data);
    if (root == NULL) {
      err_msg("Не удалось получить JSON-объект: %s\n", data);
      exit(EXIT_FAILURE);
    }

    get_info(root);
    /*current_day = json_object_object_get(root, "current_condition");
    elem = json_object_array_get_idx(current_day, current_obj_idx);
    json_object *descs = json_object_object_get(elem, "lang_ru");
    elem = json_object_array_get_idx(descs, desc_idx);
    printf("Погода - описание: %s\n", json_object_get_string(json_object_object_get(elem, "value")));*/


    
    json_object_put(root);
    return 0;
}