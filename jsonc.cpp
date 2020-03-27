#include "jsonc.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL */
#include <errno.h>  /* errno, ERANGE */
#include <math.h>   /* HUGE_VAL */
#include <string>

using namespace std;

#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')


typedef struct {
    char *json;
} Context;

// 上下文过滤器：不理想 -> 理想
static void parse_whitespace(Context *c) {
    char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

// 将json转换为null对象，并更新上下文
static ElemValue* parse_null(Context *c) {
    if (c->json[0] != 'n' || c->json[1] != 'u' || c->json[2] != 'l' || c->json[3] != 'l') {
        throw PARSE_INVALID_VALUE;
    }
    ElemValue *v = new ElemValue;
    v->type = VALUE_NULL;
    c->json += 4;
    return v;
}

// 将json转换为false对象，并更新上下文
static ElemValue* parse_false(Context *c) {
    if (c->json[0] != 'f' || c->json[1] != 'a' || c->json[2] != 'l' || c->json[3] != 's' || c->json[4] != 'e') {
        throw PARSE_INVALID_VALUE;
    }
    ElemValue *v = new ElemValue;
    v->type = VALUE_FALSE;
    c->json += 5;
    return v;
}

// 将json转换为true对象，并更新上下文
static ElemValue* parse_true(Context *c) {
    if (c->json[0] != 't' || c->json[1] != 'r' || c->json[2] != 'u' || c->json[3] != 'e') {
        throw PARSE_INVALID_VALUE;
    }
    ElemValue *v=  new ElemValue;
    v->type = VALUE_TRUE;
    c->json += 4;
    return v;
}


// 将json转换为number对象，并更新上下文
static ElemValue* parse_number(Context *c) {
    char *p = c->json;
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
    double temp = strtod(c->json, NULL);
    if (errno == ERANGE && (temp == HUGE_VAL || temp == -HUGE_VAL))
        throw PARSE_NUMBER_TOO_BIG;
    ElemValue* v = new ElemValue;
    v->n = temp;
    v->type = VALUE_NUMBER;
    c->json = p;
    return v;
}

// 将json转换为value对象, 并更新上下文
static ElemValue* parse_value(Context *c) {
    switch (*c->json) {
        case 'n':
            return parse_null(c);
        case 'f':
            return parse_false(c);
        case 't':
            return parse_true(c);
        case '\0':
            throw PARSE_EXPECT_VALUE;
        default:
            return parse_number(c);
    }
}

// 将json转换为value对象
ElemValue* parse(char *json) {
    // 建立一个上下文对象
    Context *c = new Context;
    c->json = json;

    parse_whitespace(c);
    ElemValue *v = parse_value(c);
    parse_whitespace(c);
    if (*c->json != '\0') {
        throw PARSE_ROOT_NOT_SINGULAR;
    }
    return v;
}

ValueType get_type(ElemValue *v) {
    assert(v != NULL);
    return v->type;
}

double get_number(ElemValue *v) {
    assert(v != NULL && v->type == VALUE_NUMBER);
    return v->n;
}