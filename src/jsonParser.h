#ifndef JSONCPP_JSONPARSER_H
#define JSONCPP_JSONPARSER_H

#include <vector>
#include <map>
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
    PARSE_MISS_COMMA_OR_SQUARE_BRACKET,

    PARSE_MISS_KEY,
    PARSE_MISS_COLON,
    PARSE_MISS_COMMA_OR_CURLY_BRACKET
} ExceptType;

typedef struct ElemValue {
    vector<ElemValue *> array;
    map<string, ElemValue *> maptable;
    string str;
    double n;
    ValueType type;
} ElemValue;

class Parser {
private:
    char *txt; // 上下文
    vector<ElemValue* > records; // 对象收集器，每一个创建的对象都要进行记录

    void parse_whitespace();
    ElemValue *new_value();
    ElemValue *parse_null();
    ElemValue *parse_false();
    ElemValue *parse_true();
    ElemValue *parse_number();
    ElemValue *parse_string();
    ElemValue *parse_array();
    ElemValue *parse_object();
    ElemValue *parse_value();

public:
    ElemValue *parse(char *json);
    void reset_parser();
};

#endif //JSONCPP_JSONPARSER_H
