#ifndef JSONCPP_LEPTJSON_H
#define JSONCPP_LEPTJSON_H


#include "src/jsonParser.h"

#define lept_init(v) do { (v)->type = VALUE_NULL; } while(0)

int lept_parse(ElemValue* v, const char* json);

void lept_free(ElemValue* v);

ValueType lept_get_type(const ElemValue* v);

#define lept_set_null(v) lept_free(v)

int lept_get_boolean(const ElemValue* v);
void lept_set_boolean(ElemValue* v, int b);

double lept_get_number(const ElemValue* v);
void lept_set_number(ElemValue* v, double n);

const char* lept_get_string(const ElemValue* v);
size_t lept_get_string_length(const ElemValue* v);
void lept_set_string(ElemValue* v, const char* s, size_t len);

#endif //JSONCPP_LEPTJSON_H
