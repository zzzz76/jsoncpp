#include "jsonParser.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL */
#include <errno.h>  /* errno, ERANGE */
#include <math.h>   /* HUGE_VAL */
#include <string>

using namespace std;

#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')


static ElemValue *parse_value(char *&txt);

// 上下文过滤器：不理想 -> 理想
static void parse_whitespace(char *&txt) {
    char *p = txt;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    txt = p;
}

// 将上下文转换为null对象，并更新上下文
static ElemValue *parse_null(char *&txt) {
    if (txt[0] != 'n' || txt[1] != 'u' || txt[2] != 'l' || txt[3] != 'l') {
        throw PARSE_INVALID_VALUE;
    }
    ElemValue *v = new ElemValue;
    v->type = VALUE_NULL;
    txt += 4;
    return v;
}

// 将上下文转换为false对象，并更新上下文
static ElemValue *parse_false(char *&txt) {
    if (txt[0] != 'f' || txt[1] != 'a' || txt[2] != 'l' || txt[3] != 's' || txt[4] != 'e') {
        throw PARSE_INVALID_VALUE;
    }
    ElemValue *v = new ElemValue;
    v->type = VALUE_FALSE;
    txt += 5;
    return v;
}

// 将上下文转换为true对象，并更新上下文
static ElemValue *parse_true(char *&txt) {
    if (txt[0] != 't' || txt[1] != 'r' || txt[2] != 'u' || txt[3] != 'e') {
        throw PARSE_INVALID_VALUE;
    }
    ElemValue *v = new ElemValue;
    v->type = VALUE_TRUE;
    txt += 4;
    return v;
}


// 将上下文转换为number对象，并更新上下文
static ElemValue *parse_number(char *&txt) {
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

    ElemValue *v = new ElemValue;
    v->type = VALUE_NUMBER;
    errno = 0;
    v->n = strtod(txt, NULL);
    if (errno == ERANGE && (v->n == HUGE_VAL || v->n == -HUGE_VAL)) {
        throw PARSE_NUMBER_TOO_BIG;
    }
    txt = p;
    return v;
}

// 将上下文转换为value对象，并更新上下文
static ElemValue *parse_string(char *&txt) {
    // 先为json预设value对象
    ElemValue *v = new ElemValue;
    v->type = VALUE_STRING;
    v->str = new string;
    // 游动指针
    txt++;
    while (*txt != '"') {
        if (*txt == '\0') {
            throw PARSE_MISS_QUOTATION_MARK;
        }
        v->str->push_back(*txt);
        txt++;
    }
    txt++;
    return v;
}

// 将上下文转换为value对象，并更新上下文
static ElemValue *parse_array(char *&txt) {
    // 先为json预设value对象
    ElemValue *v = new ElemValue;
    v->type = VALUE_ARRAY;
    // 游动指针
    txt++;
    while (true) {
        parse_whitespace(txt);
        v->array.push_back(parse_value(txt));
        parse_whitespace(txt);
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

// 将上下文转换为value对象, 并更新上下文
static ElemValue *parse_value(char *&txt) {
    switch (*txt) {
        case 'n':
            return parse_null(txt);
        case 'f':
            return parse_false(txt);
        case 't':
            return parse_true(txt);
        case '"':
            return parse_string(txt);
        case '[':
            return parse_array(txt);
        case '\0':
            throw PARSE_EXPECT_VALUE;
        default:
            return parse_number(txt);
    }
}

// 将json转换为value对象
ElemValue *parse(char *json) {
    // 建立一个上下文指针
    char *txt = json;
    parse_whitespace(txt);
    ElemValue *v = parse_value(txt);
    parse_whitespace(txt);
    if (*txt != '\0') {
        throw PARSE_ROOT_NOT_SINGULAR;
    }
    return v;
}

//ValueType get_type(ElemValue *v) {
//    assert(v != NULL);
//    return v->type;
//}
//
//double get_number(ElemValue *v) {
//    assert(v != NULL && v->type == VALUE_NUMBER);
//    return v->n;
//}
//
//string* get_string(ElemValue *v) {
//    assert(v != NULL && v->type == VALUE_STRING);
//    return v->str;
//}

