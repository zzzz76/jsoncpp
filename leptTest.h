#ifndef JSONCPP_LEPTTEST_H
#define JSONCPP_LEPTTEST_H


#include "jsonc.h"

int lept_parse(ElemValue *v, const char *json);

ValueType lept_get_type(ElemValue* v);

double lept_get_number(ElemValue* v);

#endif //JSONCPP_LEPTTEST_H
