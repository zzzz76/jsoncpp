#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsonc.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do {\
        test_count++;\
        if (equality)\
            test_pass++;\
        else {\
            fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
            main_ret = 1;\
        }\
    } while(0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

static void test_parse_null() {
    ElemValue v;
    v.type = VALUE_FALSE;
    EXPECT_EQ_INT(PARSE_OK, parse(&v, "null"));
    EXPECT_EQ_INT(VALUE_NULL, get_type(&v));
}

static void test_parse_expect_value() {
    ElemValue v;

    v.type = VALUE_FALSE;
    EXPECT_EQ_INT(PARSE_EXPECT_VALUE, parse(&v, ""));
    EXPECT_EQ_INT(VALUE_NULL, get_type(&v));

    v.type = VALUE_FALSE;
    EXPECT_EQ_INT(PARSE_EXPECT_VALUE, parse(&v, " "));
    EXPECT_EQ_INT(VALUE_NULL, get_type(&v));
}

static void test_parse_invalid_value() {
    ElemValue v;
    v.type = VALUE_FALSE;
    EXPECT_EQ_INT(PARSE_INVALID_VALUE, parse(&v, "nul"));
    EXPECT_EQ_INT(VALUE_NULL, get_type(&v));

    v.type = VALUE_FALSE;
    EXPECT_EQ_INT(PARSE_INVALID_VALUE, parse(&v, "?"));
    EXPECT_EQ_INT(VALUE_NULL, get_type(&v));
}

static void test_parse_root_not_singular() {
    ElemValue v;
    v.type = VALUE_FALSE;
    EXPECT_EQ_INT(PARSE_ROOT_NOT_SINGULAR, parse(&v, "null x"));
    EXPECT_EQ_INT(VALUE_NULL, get_type(&v));
}

static void test_parse() {
    test_parse_null();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
}

int main() {
    test_parse();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}