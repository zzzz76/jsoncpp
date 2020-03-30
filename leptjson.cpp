#include "leptjson.h"
#include <assert.h>

int lept_parse(ElemValue *v, const char *json) {
    try {
        lept_free(v);
        *v = *Parser().parse(const_cast<char *>(json));
    } catch (ExceptType e) {
        return e;
    }
    return PARSE_OK;
}

void lept_free(ElemValue* v) {
    assert(v != NULL);
    if (v->type == VALUE_STRING)
        delete(v->str);
    v->type = VALUE_NULL;
}

ValueType lept_get_type(const ElemValue* v) {
    assert(v != NULL);
    return v->type;
}

int lept_get_boolean(const ElemValue* v) {
    assert(v != NULL && (v->type == VALUE_TRUE || v->type == VALUE_FALSE));
    return v->type == VALUE_TRUE;
}

void lept_set_boolean(ElemValue* v, int b) {
    lept_free(v);
    v->type = b ? VALUE_TRUE : VALUE_FALSE;
}

double lept_get_number(const ElemValue* v) {
    assert(v != NULL && v->type == VALUE_NUMBER);
    return v->n;
}

void lept_set_number(ElemValue* v, double n) {
    lept_free(v);
    v->n = n;
    v->type = VALUE_NUMBER;
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
