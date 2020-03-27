#include <assert.h>
#include "jsonc.h"
#include <cstdlib>
#include <cstdio>

int lept_parse(ElemValue *v, const char *json) {
    try {
        *v = *parse(const_cast<char *>(json));
    } catch (ExceptType e) {
        return e;
    }
    return PARSE_OK;
}

ValueType lept_get_type(ElemValue* v) {
    return get_type(v);
}

double lept_get_number(ElemValue* v) {
    return get_number(v);
}