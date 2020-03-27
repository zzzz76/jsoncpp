#ifndef JSONCPP_JSONC_H
#define JSONCPP_JSONC_H

typedef enum { VALUE_NULL, VALUE_FALSE, VALUE_TRUE, VALUE_NUMBER, VALUE_STRING, VALUE_ARRAY, VALUE_OBJECT } ValueType;

typedef struct {
    double n;
    ValueType type;
}ElemValue;

typedef enum {
    PARSE_OK = 0,
    PARSE_EXPECT_VALUE,
    PARSE_INVALID_VALUE,
    PARSE_ROOT_NOT_SINGULAR,
    PARSE_NUMBER_TOO_BIG
} ExceptType;

ElemValue* parse(char* json);

ValueType get_type(ElemValue* v);

double get_number(ElemValue* v);

#endif //JSONCPP_JSONC_H
