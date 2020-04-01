#ifndef JSONCPP_JSONPARSER_H
#define JSONCPP_JSONPARSER_H

#include "jsonValue.h"

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

class Parser {
private:
    char *txt; // 上下文
    vector<Value* > tmp; // 对象缓存器

    Parser(char *json);
    void parse_whitespace();
    Value *new_value();
    Value *parse_null();
    Value *parse_false();
    Value *parse_true();
    Value *parse_number();
    Value *parse_string();
    Value *parse_array();
    Value *parse_object();
    Value *parse_value();
public:
    static Value *parse(char *json);
};

#endif //JSONCPP_JSONPARSER_H
