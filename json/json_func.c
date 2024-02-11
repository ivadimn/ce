#include "json_func.h"
#include <stdio.h>


w_info_t w_descr = {.key = "lang_ru", .value = "value",
          .next = NULL, .format = "Погода - описание: %s\n"} ;

w_info_t winfo[] =  {
      {.key = "current_condition", .value = NULL, .next = &w_descr, .format = NULL},
      {.key = "current_condition", .value = "temp_C", 
               .next = NULL, .format = "Температура: %s гр. Цельсия\n"},
      {.key = "current_condition", .value = "winddir16Point", 
               .next = NULL, .format = "Направление ветра: %s\n"},
      {.key = "current_condition", .value = "windspeedKmph",
               .next = NULL, .format = "Скорость ветра  %s км. ч.\n"},
      {.key = "weather", .value = "maxtempC",  
               .next = NULL, .format = "Максимальная температура: %s гр. Цельсия\n"},
      {.key = "weather", .value = "mintempC", 
               .next = NULL, .format = "Минимальная температура: %s гр. Цельсия\n"},
      };

void get_info(json_object *root) {
   size_t count = 6, first = 0;
   json_object *up = NULL, *elem = NULL;
   for (size_t i = 0; i < count; i++) {
      up = json_object_object_get(root, winfo[i].key);
      if (winfo[i].next != NULL) {
         w_info_t *next = winfo[i].next;
         elem = json_object_array_get_idx(up, first);
         up = json_object_object_get(elem, next->key);
         elem = json_object_array_get_idx(up, first);
         printf(next->format, json_object_get_string(json_object_object_get(elem, next->value)));
      }
      else {
         elem = json_object_array_get_idx(up, first);
         printf(winfo[i].format, json_object_get_string(json_object_object_get(elem, winfo[i].value)));
      }
   }
   

}

void json_object_print_type(json_object *obj, const char *key)
{
   json_type type;

   type = json_object_get_type(obj);  /*Getting the type of the json object*/
   switch (type)
      {
       case json_type_null:
          printf("%s is json_type_null\n", key);
          printf("          value: %s\n", json_object_get_string(obj));
          break;

       case json_type_boolean:
          printf("%s is json_type_boolean\n", key);
          printf("          value: %s\n", json_object_get_boolean(obj)? "true": "false");
          break;

       case json_type_double:
          printf("%s is json_type_double\n", key);
          printf("          value: %lf\n", json_object_get_double(obj));
          break;

       case json_type_int:
          printf("%s is json_type_int\n", key);
          printf("          value: %d\n", json_object_get_int(obj));
          break;

       case json_type_object:
          printf("%s is json_type_object\n", key);
          printf("          value: %s\n", json_object_get_string(obj));
          break;

       case json_type_array:
          printf("%s is json_type_array\n", key);
          printf("          value: %s\n", json_object_get_string(obj));
          break;

       case json_type_string:
          printf("%s is json_type_string\n", key);
          printf("          value: %s\n", json_object_get_string(obj));
          break;

       default:
          printf("%s: WTF, unhandled case, type %d\n", key, type);
          printf("          value: %s\n", json_object_get_string(obj));
      }
}