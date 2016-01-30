#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <jansson.h>
using namespace std;

//////////////////////////////////////////////////////////////////////////////////
// refer https://github.com/akheron/jansson/blob/master/examples/simple_parse.c //
/* forward refs */
void print_json(json_t *root);
void print_json_aux(json_t *element, int indent);
void print_json_indent(int indent);
const char *json_plural(int count);
void print_json_object(json_t *element, int indent);
void print_json_array(json_t *element, int indent);
void print_json_string(json_t *element, int indent);
void print_json_integer(json_t *element, int indent);
void print_json_real(json_t *element, int indent);
void print_json_true(json_t *element, int indent);
void print_json_false(json_t *element, int indent);
void print_json_null(json_t *element, int indent);

void print_json(json_t *root) {
    printf("{");
    print_json_aux(root, 0);
    printf("}\n");
}

void print_json_aux(json_t *element, int indent) {
    switch (json_typeof(element)) {
    case JSON_OBJECT:
        print_json_object(element, indent);
        break;
    case JSON_ARRAY:
        print_json_array(element, indent);
        break;
    case JSON_STRING:
        print_json_string(element, indent);
        break;
    case JSON_INTEGER:
        print_json_integer(element, indent);
        break;
    case JSON_REAL:
        print_json_real(element, indent);
        break;
    case JSON_TRUE:
        print_json_true(element, indent);
        break;
    case JSON_FALSE:
        print_json_false(element, indent);
        break;
    case JSON_NULL:
        print_json_null(element, indent);
        break;
    default:
        fprintf(stderr, "unrecognized JSON type %d\n", json_typeof(element));
    }
}

void print_json_indent(int indent) {
    int i;
    for (i = 0; i < indent; i++) { putchar(' '); }
}

const char *json_plural(int count) {
    return count == 1 ? "" : "s";
}

void print_json_object(json_t *element, int indent) {
    size_t size;
    const char *key;
    json_t *value;

    print_json_indent(indent);
    //size = json_object_size(element);

    //printf("JSON Object of %ld pair%s:\n", size, json_plural(size));
    json_object_foreach(element, key, value) {
        print_json_indent(indent + 2);
        printf("\"%s\":", key);
        print_json_aux(value, indent + 2);
    }

}

void print_json_array(json_t *element, int indent) {
    size_t i;
    size_t size = json_array_size(element);
    print_json_indent(indent);

    //printf("JSON Array of %ld element%s:\n", size, json_plural(size));
    for (i = 0; i < size; i++) {
        print_json_aux(json_array_get(element, i), indent + 2);
    }
}

void print_json_string(json_t *element, int indent) {
    print_json_indent(indent);
    printf("\"%s\",", json_string_value(element));
}

void print_json_integer(json_t *element, int indent) {
    print_json_indent(indent);
    printf("\"%" JSON_INTEGER_FORMAT "\",", json_integer_value(element));
}

void print_json_real(json_t *element, int indent) {
    print_json_indent(indent);
    printf("%f,", json_real_value(element));
}

void print_json_true(json_t *element, int indent) {
    (void)element;
    print_json_indent(indent);
    printf("JSON True\n");
}

void print_json_false(json_t *element, int indent) {
    (void)element;
    print_json_indent(indent);
    printf("JSON False\n");
}

void print_json_null(json_t *element, int indent) {
    (void)element;
    print_json_indent(indent);
    printf("JSON Null\n");
}
//////////////////////////////////////////////////////////////////////////////////////////////

int get_meta_data(const char *raw_meta, const char *primary_id_type, const char *secondary_id_type, json_t *result_meta) {
  json_error_t error;
  json_t *parsed;
  json_t *service;
  json_t *contentidlist;
  json_t *contentid;
  json_t *key_obj;
  json_t *candidate = json_array();
  
  const char *contentIdList = "contentIdList";
  const char *type = "type";
  const char *custom = "Custom";
  const char *serviceCode = "serviceCode";
  const char *idType = "idType";
  const char *contentId = "contentId";
  const char *contentTitle = "contentTitle";
  const char *altIdType = "altIdType";
  const char *altcontentId = "altContentId";
  const char *altcontentTitle = "altContentTitle";
  const char *key;

  bool b_primary = false;
  bool b_secondary = false;
  bool b_custom = false;

  int result = 0;
  parsed = json_loads(raw_meta, 0, &error);
  if (parsed) {
    contentidlist = json_object_get(parsed, contentIdList);
    service = json_object_get(parsed, serviceCode);
    json_object_set(result_meta, serviceCode, service);

    size_t size = json_array_size(contentidlist);
    size_t i;
    //cout<< size << endl;
    for (i = 0; i < size; i++) {
    	contentid = json_array_get(contentidlist, i);
        key_obj = json_object_get(contentid, type);
        key = json_string_value(key_obj);
        if ( strcmp(key, primary_id_type) == 0){
          b_primary = true;
          json_array_append(candidate, contentid);
          result ++;
        }
        else if (strcmp(key, secondary_id_type) == 0 ){
          b_secondary = true;
          json_array_append(candidate, contentid);
          result ++;
        }
        else if (strcmp(key, custom)==0){
          b_custom = true;
          json_array_append(candidate, contentid);
          result ++;
        }
    }

    size = json_array_size(candidate);
    for (i = 0; i < size; i++) {
        contentid = json_array_get(candidate, i);
        key_obj = json_object_get(contentid, type);
        key = json_string_value(key_obj);
        if(b_primary){
          if( strcmp(key, primary_id_type) == 0){
              json_object_set(result_meta, idType, key_obj);
              key_obj = json_object_get(contentid, contentId);
              json_object_set(result_meta, contentId, key_obj);
              key_obj = json_object_get(contentid, contentTitle);
              json_object_set(result_meta, contentTitle, key_obj); 
          }
          if (b_secondary){
            if( strcmp(key, secondary_id_type) == 0){
              json_object_set(result_meta, altIdType, key_obj);
              key_obj = json_object_get(contentid, contentId);
              json_object_set(result_meta, altcontentId, key_obj);
              key_obj = json_object_get(contentid, contentTitle);
              json_object_set(result_meta, altcontentTitle, key_obj); 
            }
          } else {
              if( strcmp(key, custom) == 0){
                json_object_set(result_meta, altIdType, key_obj);
                key_obj = json_object_get(contentid, contentId);
                json_object_set(result_meta, altcontentId, key_obj);
                key_obj = json_object_get(contentid, contentTitle);
                json_object_set(result_meta, altcontentTitle, key_obj); 
              }
          }
        } else {
          if (b_secondary){
            if( strcmp(key, secondary_id_type) == 0){
              json_object_set(result_meta, idType, key_obj);
              key_obj = json_object_get(contentid, contentId);
              json_object_set(result_meta, contentId, key_obj);
              key_obj = json_object_get(contentid, contentTitle);
              json_object_set(result_meta, contentTitle, key_obj); 
            }
            if(b_custom){
              if( strcmp(key, custom) == 0){
                json_object_set(result_meta, altIdType, key_obj);
                key_obj = json_object_get(contentid, contentId);
                json_object_set(result_meta, altcontentId, key_obj);
                key_obj = json_object_get(contentid, contentTitle);
                json_object_set(result_meta, altcontentTitle, key_obj); 
              }
            }
          } else {
            if( strcmp(key, custom) == 0){
              json_object_set(result_meta, idType, key_obj);
              key_obj = json_object_get(contentid, contentId);
              json_object_set(result_meta, contentId, key_obj);
              key_obj = json_object_get(contentid, contentTitle);
              json_object_set(result_meta, contentTitle, key_obj); 
            }
          }
        }
    }
       
    return result;
  } else {
    fprintf(stderr, "json error on line %d: %s\n", error.line, error.text);
    return 0;
  }
}

int main(int argc, char **argv) {
  string input_line;
  
  //check argv count 
  if (argc != 3){
    cout << "[ERROR] please enter the 2 arguments" << endl;
    return 0;
  }	

  while (cin) {
    getline(cin, input_line);
    //char *res = (char*)malloc(input_line.size() + 128);
    json_t *res = json_object();
    int r = get_meta_data(input_line.c_str(), argv[1], argv[2], res);
    if ( r > 0 ) {
      print_json(res);
    }
  }
}
