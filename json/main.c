#include <stdio.h>
#include <curl/curl.h>
#include <json-c/json.h>

void json_parse_array( json_object *jobj, char *key);

/*printing the value corresponding to boolean, double, integer and strings*/
void print_json_value(json_object *jobj){
  enum json_type type;
  printf("type: ",type);
  type = json_object_get_type(jobj); /*Getting the type of the json object*/
  switch (type) {
    case json_type_boolean: printf("json_type_boolean\n");
                         printf("value: %sn", json_object_get_boolean(jobj)? "true": "false");
                         break;
    case json_type_double: printf("json_type_double\n");
                        printf("          value: %lf\n", json_object_get_double(jobj));
                         break;
    case json_type_int: printf("json_type_int\n");
                        printf("          value: %d\n", json_object_get_int(jobj));
                         break;
    case json_type_string: printf("json_type_string\n");
                         printf("          value: %s\n", json_object_get_string(jobj));
                         break;
  }

}


/*Parsing the json object*/
void json_parse(json_object * jobj) {
  enum json_type type;
  json_object_object_foreach(jobj, key, val) { /*Passing through every array element*/
    printf("type: ",type);
    type = json_object_get_type(val);
    switch (type) {
      case json_type_boolean: 
      case json_type_double: 
      case json_type_int: 
      case json_type_string: print_json_value(val);
                           break; 
      case json_type_object: printf("json_type_object\n");
                           jobj = json_object_object_get(jobj, key);
                           json_parse(jobj); 
                           break;
      case json_type_array: printf("type: json_type_array, \n");
                          json_parse_array(jobj, key);
                          break;
    }
  }
} 

void json_parse_array( json_object *jobj, char *key) {
  
  enum json_type type;

  json_object *jarray = jobj; /*Simply get the array*/
  if(key) {
    jarray = json_object_object_get(jobj, key); /*Getting the array if it is a key value pair*/
  }

  int arraylen = json_object_array_length(jarray); /*Getting the length of the array*/
  printf("Array Length: %dn",arraylen);
  int i;
  json_object * jvalue;

  for (i=0; i< arraylen; i++){
    jvalue = json_object_array_get_idx(jarray, i); /*Getting the array element at position i*/
    type = json_object_get_type(jvalue);
    if (type == json_type_array) {
      json_parse_array(jvalue, NULL);
    }
    else if (type != json_type_object) {
      printf("value[%d]: ",i);
      print_json_value(jvalue);
    }
    else {
      json_parse(jvalue);
    }
  }
}

int main(int argc, char** argv)
{
    
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

    char * string = "{\"sitename\" : \"joys of programming\", \
                     \"categories\" : [ \"c\" , [\"c++\" , \"c\" ], \"java\", \"PHP\" ], \
                     \"author-details\": { \"admin\": false, \"name\" : \"Joys of Programming\", \"Number of Posts\" : 10} \
                     }";
    printf("JSON string: %sn", string);
    json_object * jobj = json_tokener_parse(string);     
    json_parse(jobj);

    return 0;
}