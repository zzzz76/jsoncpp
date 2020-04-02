#include "jsonParser.h"

#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')


Parser::Parser(char *json) {
    collector.clear();
    txt = json;
}

// 收集value对象
Value* Parser::collect(Value *v) {
    collector.push_back(v);
    return v;
}


// 解析文本，过滤空白字符
void Parser::parse_whitespace() {
    char *p = txt;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    txt = p;
}

// 解析文本，并返回null对象
Value *Parser::parse_null() {
    if (txt[0] != 'n' || txt[1] != 'u' || txt[2] != 'l' || txt[3] != 'l') {
        throw PARSE_INVALID_VALUE;
    }
    txt += 4;
    return collect(new Value());
}

// 解析文本，并返回false对象
Value *Parser::parse_false() {
    if (txt[0] != 'f' || txt[1] != 'a' || txt[2] != 'l' || txt[3] != 's' || txt[4] != 'e') {
        throw PARSE_INVALID_VALUE;
    }
    txt += 5;
    return collect(new Value(false));
}


// 解析文本，并返回true对象
Value *Parser::parse_true() {
    if (txt[0] != 't' || txt[1] != 'r' || txt[2] != 'u' || txt[3] != 'e') {
        throw PARSE_INVALID_VALUE;
    }
    txt += 4;
    return collect(new Value(true));
}

// 解析文本，并返回number对象
Value *Parser::parse_number() {
    char *p = txt;
    //负号
    if (*p == '-') p++;
    //整数
    if (*p == '0') {
        p++;
    } else if (ISDIGIT1TO9(*p)) {
        p++;
        for (p; ISDIGIT(*p); ++p);
    } else {
        throw PARSE_INVALID_VALUE;
    }
    //小数
    if (*p == '.') {
        p++;
        if (ISDIGIT(*p)) {
            p++;
            for (p; ISDIGIT(*p); ++p);
        } else {
            throw PARSE_INVALID_VALUE;
        }
    }
    //指数
    if (*p == 'e' || *p == 'E') {
        p++;
        if (*p == '+' || *p == '-') p++;
        if (ISDIGIT(*p)) {
            p++;
            for (p; ISDIGIT(*p); p++);
        } else {
            throw PARSE_INVALID_VALUE;
        }
    }
    double num = strtod(txt, NULL);
    txt = p;
    return collect(new Value(num));
}

// 解析文本，并返回string对象
Value *Parser::parse_string() {
    // 解析文本
    string str;
    // 游动指针
    txt++;
    while (*txt != '"') {
        if (*txt == '\0') {
            throw PARSE_MISS_QUOTATION_MARK;
        }
        str.push_back(*txt);
        txt++;
    }
    txt++;
    return collect(new Value(str));
}

// 解析文本，并返回array对象
Value *Parser::parse_array() {
    // 解析文本
    vector<Value *> array;
    // 游动指针
    txt++;
    parse_whitespace();
    if (*txt == ']') {
        txt++;
        return collect(new Value(array));
    }
    while (true) {
        array.push_back(parse_value());
        parse_whitespace();
        if (*txt == ',') {
            txt++;
            parse_whitespace();
        } else if (*txt == ']') {
            txt++;
            return collect(new Value(array));
        } else {
            throw PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
        }
    }
}

// 解析文本，并返回object对象
Value* Parser::parse_object() {
    // 解析文本
    map<string, Value *> object;
    // 游动指针
    txt++;
    parse_whitespace();
    if (*txt == '}') {
        txt++;
        return collect(new Value(object));
    }
    while (true) {
        if (*txt != '"') {
            throw PARSE_MISS_KEY;
        }
        txt++;
        //开始解析字符串
        pair<string, Value *> p;
        while (*txt != '"') {
            if (*txt == '\0') {
                throw PARSE_MISS_QUOTATION_MARK;
            }
            p.first.push_back(*txt);
            txt++;
        }
        txt++;
        parse_whitespace();
        if (*txt != ':') {
            throw PARSE_MISS_COLON;
        }
        txt++;
        parse_whitespace();
        p.second = parse_value();
        object.insert(p);
        parse_whitespace();
        if (*txt == ',') {
            txt++;
            parse_whitespace();
        } else if (*txt == '}') {
            txt++;
            return collect(new Value(object));
        } else {
            throw PARSE_MISS_COMMA_OR_CURLY_BRACKET;
        }
    }
}

// 解析文本，并返回对应的对象
Value *Parser::parse_value() {
    switch (*txt) {
        case 'n':
            return parse_null();
        case 'f':
            return parse_false();
        case 't':
            return parse_true();
        case '"':
            return parse_string();
        case '[':
            return parse_array();
        case '{':
            return parse_object();
        case '\0':
            throw PARSE_EXPECT_VALUE;
        default:
            return parse_number();
    }
}

// 将json字符串转换为json对象
Value *Parser::parse(char *json) {
    Parser parser(json);
    try {
        parser.parse_whitespace();
        Value *v = parser.parse_value();
        parser.parse_whitespace();
        if (*parser.txt != '\0') {
            throw PARSE_ROOT_NOT_SINGULAR;
        }
        return v;
    } catch (ExceptType e){
        for (auto &it : parser.collector) {
            delete(it);
        }
        throw e;
    }
}
