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

class Parser {
private:
    char *txt;
    void parse_whitespace();
    ElemValue *parse_null();
    ElemValue *parse_false();
    ElemValue *parse_true();
    ElemValue *parse_number();
    ElemValue *parse_string();
    ElemValue *parse_array();
    ElemValue *parse_value();

public:
    ElemValue *parse(char *json);
};

#endif //JSONCPP_JSONPARSER_H
