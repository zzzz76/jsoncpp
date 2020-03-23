#ifndef JSONCPP_JSONC_H
#define JSONCPP_JSONC_H

typedef enum { VALUE_NULL, VALUE_FALSE, VALUE_TRUE, VALUE_NUMBER, VALUE_STRING, VALUE_ARRAY, VALUE_OBJECT } ValueType;

typedef struct {
    ValueType type;
}ElemValue;

enum {
    PARSE_OK = 0,
    PARSE_EXPECT_VALUE,
    PARSE_INVALID_VALUE,
    PARSE_ROOT_NOT_SINGULAR
};

int parse(ElemValue* v, const char* json);

ValueType get_type(const ElemValue* v);

#endif //JSONCPP_JSONC_H
