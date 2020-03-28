#include <assert.h>
#include "jsonParser.h"
#include "leptjson.h"

int lept_parse(ElemValue *v, const char *json) {
    try {
        *v = *parse(const_cast<char *>(json));
    } catch (ExceptType e) {
        lept_init(v);
        return e;
    }
    return PARSE_OK;
}

void lept_free(ElemValue* v) {
    assert(v != NULL);
    if (v->type == VALUE_STRING)
        free(v->str);
    v->type = VALUE_NULL;
}

ValueType lept_get_type(const ElemValue* v) {
    assert(v != NULL);
    return v->type;
}

int lept_get_boolean(const ElemValue* v) {
    /* \TODO */
    return 0;
}

void lept_set_boolean(ElemValue* v, int b) {
    /* \TODO */
}

double lept_get_number(const ElemValue* v) {
    assert(v != NULL && v->type == VALUE_NUMBER);
    return v->n;
}

void lept_set_number(ElemValue* v, double n) {
    /* \TODO */
}

const char* lept_get_string(const ElemValue* v) {
    assert(v != NULL && v->type == VALUE_STRING);
    return v->str->c_str();
}

size_t lept_get_string_length(const ElemValue* v) {
    assert(v != NULL && v->type == VALUE_STRING);
    return v->str->size();
}

void lept_set_string(ElemValue* v, const char* s, size_t len) {
    assert(v != NULL && (s != NULL || len == 0));
    lept_free(v);
    v->str = new string(s);
    v->type = VALUE_STRING;
}
