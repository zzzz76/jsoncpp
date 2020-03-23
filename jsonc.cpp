#include "jsonc.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL */

#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)

typedef struct {
    const char* json;
}Context;

static void parse_whitespace(Context* c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

static int parse_null(Context* c, ElemValue* v) {
    EXPECT(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = VALUE_NULL;
    return PARSE_OK;
}

static int parse_value(Context* c, ElemValue* v) {
    switch (*c->json) {
        case 'n':  return parse_null(c, v);
        case '\0': return PARSE_EXPECT_VALUE;
        default:   return PARSE_INVALID_VALUE;
    }
}

int parse(ElemValue* v, const char* json) {
    Context c;
    assert(v != NULL);
    c.json = json;
    v->type = VALUE_NULL;
    parse_whitespace(&c);
    return parse_value(&c, v);
}

ValueType get_type(const ElemValue* v) {
    assert(v != NULL);
    return v->type;
}