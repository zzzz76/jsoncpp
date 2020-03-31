#include "jsonParser.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL */
#include <errno.h>  /* errno, ERANGE */
#include <math.h>   /* HUGE_VAL */

using namespace std;

#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')


// 创建一个value对象
ElemValue* Parser::new_value() {
    ElemValue *v = new ElemValue;
    records.push_back(v);
    return v;
}

// 上下文过滤器：不理想 -> 理想
void Parser::parse_whitespace() {
    char *p = txt;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    txt = p;
}

// 将上下文转换为null对象，并更新上下文
ElemValue *Parser::parse_null() {
    if (txt[0] != 'n' || txt[1] != 'u' || txt[2] != 'l' || txt[3] != 'l') {
        throw PARSE_INVALID_VALUE;
    }
    ElemValue *v = new_value();
    v->type = VALUE_NULL;
    txt += 4;
    return v;
}

// 将上下文转换为false对象，并更新上下文
ElemValue *Parser::parse_false() {
    if (txt[0] != 'f' || txt[1] != 'a' || txt[2] != 'l' || txt[3] != 's' || txt[4] != 'e') {
        throw PARSE_INVALID_VALUE;
    }
    ElemValue *v = new_value();
    v->type = VALUE_FALSE;
    txt += 5;
    return v;
}

// 将上下文转换为true对象，并更新上下文
ElemValue *Parser::parse_true() {
    if (txt[0] != 't' || txt[1] != 'r' || txt[2] != 'u' || txt[3] != 'e') {
        throw PARSE_INVALID_VALUE;
    }
    ElemValue *v = new_value();
    v->type = VALUE_TRUE;
    txt += 4;
    return v;
}


// 将上下文转换为number对象，并更新上下文
ElemValue *Parser::parse_number() {
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

    errno = 0;
    double temp = strtod(txt, NULL);
    if (errno == ERANGE && (temp == HUGE_VAL || temp == -HUGE_VAL)) {
        throw PARSE_NUMBER_TOO_BIG;
    }
    ElemValue *v = new_value();
    v->type = VALUE_NUMBER;
    v->n = temp;
    txt = p;
    return v;
}

// 将上下文转换为value对象，并更新上下文
ElemValue *Parser::parse_string() {
    // 先为json预设value对象
    ElemValue *v = new_value();
    v->type = VALUE_STRING;
    // 游动指针
    txt++;
    while (*txt != '"') {
        if (*txt == '\0') {
            throw PARSE_MISS_QUOTATION_MARK;
        }
        v->str.push_back(*txt);
        txt++;
    }
    txt++;
    return v;
}

// 将上下文转换为value对象，并更新上下文
ElemValue *Parser::parse_array() {
    // 先为json预设value对象
    ElemValue *v = new_value();
    v->type = VALUE_ARRAY;
    // 游动指针
    txt++;
    while (true) {
        parse_whitespace();
        v->array.push_back(parse_value());
        parse_whitespace();
        if (*txt == ',') {
            txt++;
        } else if (*txt == ']') {
            txt++;
            break;
        } else {
            throw PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
        }
    }
    return v;
}

// 将上下文转换为value对象，并更新上下文
ElemValue* Parser::parse_object() {
    // 先为json预设value对象
    ElemValue *v = new_value();
    v->type = VALUE_OBJECT;
    // 游动指针
    txt++;
    while (true) {
        parse_whitespace();
        if (*txt != '"') {
            throw PARSE_MISS_KEY;
        }
        txt++;
        //开始解析字符串
        pair<string, ElemValue *> p;
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
        v->maptable.insert(p);
        parse_whitespace();
        if (*txt == ',') {
            txt++;
        } else if (*txt == '}') {
            txt++;
            break;
        } else {
            throw PARSE_MISS_COMMA_OR_CURLY_BRACKET;
        }
    }
    return v;
}

// 将上下文转换为value对象, 并更新上下文
ElemValue *Parser::parse_value() {
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

// 将json转换为value对象
ElemValue *Parser::parse(char *json) {
    try {
        // 建立一个上下文指针
        txt = json;
        parse_whitespace();
        ElemValue *v = parse_value();
        parse_whitespace();
        if (*txt != '\0') {
            throw PARSE_ROOT_NOT_SINGULAR;
        }
        return v;
    } catch (ExceptType e){
        reset_parser();
        throw e;
    }

}

// 重置解释器
void Parser::reset_parser() {
    for (auto &record : records) {
        delete(record);
    }
    records.clear();
    txt = nullptr;
}