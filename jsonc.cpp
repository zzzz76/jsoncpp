#include "jsonc.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL */
#include <errno.h>  /* errno, ERANGE */
#include <math.h>   /* HUGE_VAL */

#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)
#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')


typedef struct {
    const char* json;
}Context;

// 上下文过滤器：不理想 -> 理想
static void parse_whitespace(Context* c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

// 解析理想的上下文字段，并且返回结果
static int parse_literal(Context* c, ElemValue* v, const char* literal, ValueType type) {
    size_t i;
    EXPECT(c, literal[0]);
    for (i = 0; literal[i + 1] != '\0'; i++ ) {
        if (c->json[i] != literal[i + 1]) {
            return PARSE_INVALID_VALUE;
        }
    }
    c->json += i;
    v->type = type;
    return PARSE_OK;
}

// 解析理想的上下文数字，并且返回结果
static int parse_number(Context* c, ElemValue* v) {
    const char* p = c->json;
    //负号
    if (*p == '-') p++;
    //整数
    if (*p == '0') {
        p++;
    } else if (ISDIGIT1TO9(*p)){
        p++;
        for (p; ISDIGIT(*p); ++p);
    } else {
        return PARSE_INVALID_VALUE;
    }
    //小数
    if (*p == '.') {
        p++;
        if (ISDIGIT(*p)) {
            p++;
            for (p; ISDIGIT(*p); ++p);
        } else {
            return PARSE_INVALID_VALUE;
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
            return PARSE_INVALID_VALUE;
        }
    }

    errno = 0;
    v->n = strtod(c->json, NULL);
    if (errno == ERANGE && (v->n == HUGE_VAL || v->n == -HUGE_VAL))
        return PARSE_NUMBER_TOO_BIG;
    c->json = p;
    v->type = VALUE_NUMBER;
    return PARSE_OK;
}

// 解析理想的上下文，并且返回结果
static int parse_value(Context* c, ElemValue* v) {
    switch (*c->json) {
        case 'n':  return parse_literal(c, v, "null", VALUE_NULL);
        case 'f':  return parse_literal(c, v, "false", VALUE_FALSE);
        case 't':  return parse_literal(c, v, "true", VALUE_TRUE);
        case '\0': return PARSE_EXPECT_VALUE;
        default:   return parse_number(c, v);
    }
}

// 解析不理想的上下文，并返回结果
int parse(ElemValue* v, const char* json) {
    // 建立一个上下文对象
    Context c;
    assert(v != NULL);
    c.json = json;

    // 开始对v对象进行解析，并返回解析结果
    v->type = VALUE_NULL;
    parse_whitespace(&c);
    int res =  parse_value(&c, v);
    parse_whitespace(&c);
    if (res == PARSE_OK && *c.json != '\0') {
        v->type = VALUE_NULL;
        return PARSE_ROOT_NOT_SINGULAR;
    }
    return res;
}

ValueType get_type(const ElemValue* v) {
    assert(v != NULL);
    return v->type;
}

double get_number(const ElemValue* v) {
    assert(v != NULL && v->type == VALUE_NUMBER);
    return v->n;
}