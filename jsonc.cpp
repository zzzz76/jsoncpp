#include "jsonc.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL */

#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)

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

// 解析理想的上下文null，并且返回结果
static int parse_null(Context* c, ElemValue* v) {
    EXPECT(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = VALUE_NULL;
    return PARSE_OK;
}

// 解析理想的上下文false，并且返回结果
static int parse_false(Context* c, ElemValue* v) {
    EXPECT(c, 'f');
    if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e') {
        return PARSE_INVALID_VALUE;
    }
    c->json += 4;
    v->type = VALUE_FALSE;
    return PARSE_OK;
}

// 解析理想的上下文true，并且返回结果
static int parse_true(Context* c, ElemValue* v) {
    // 解析v 并返回解析结果
    EXPECT(c, 't');
    if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e') {
        return PARSE_INVALID_VALUE;
    }
    c->json += 3;
    v->type = VALUE_TRUE;
    return PARSE_OK;
}

// 解析理想的上下文数字，并且返回结果
static int parse_number(Context* c, ElemValue* v) {
    char* p;
    v->n = strtod(c->json, &p);
    if (c->json == p) {
        return PARSE_INVALID_VALUE;
    }
    c->json = p;
    v->type = VALUE_NUMBER;
    return PARSE_OK;
}

// 解析理想的上下文，并且返回结果
static int parse_value(Context* c, ElemValue* v) {
    switch (*c->json) {
        case 'n':  return parse_null(c, v);
        case 'f':  return parse_false(c, v);
        case 't':  return parse_true(c, v);
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