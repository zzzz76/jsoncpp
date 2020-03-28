#ifndef JSONCPP_JSONPARSER_H
#define JSONCPP_JSONPARSER_H

#include <vector>
#include <string>

using namespace std;

typedef enum {
    VALUE_NULL, VALUE_FALSE, VALUE_TRUE, VALUE_NUMBER, VALUE_STRING, VALUE_ARRAY, VALUE_OBJECT
} ValueType;

typedef enum {
    PARSE_OK = 0,
    PARSE_EXPECT_VALUE,
    PARSE_INVALID_VALUE,
    PARSE_ROOT_NOT_SINGULAR,
    PARSE_NUMBER_TOO_BIG,

    PARSE_MISS_QUOTATION_MARK,
    PARSE_MISS_COMMA_OR_SQUARE_BRACKET
} ExceptType;

typedef struct ElemValue {
    vector<ElemValue *> array;
    string *str;
    double n;
    ValueType type;
} ElemValue;

ElemValue *parse(char *json);

//ValueType get_type(ElemValue *v);
//
//double get_number(ElemValue *v);
//
//string* get_string(ElemValue *v)

#endif //JSONCPP_JSONPARSER_H
