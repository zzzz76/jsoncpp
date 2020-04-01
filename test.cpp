#include "src/jsonParser.h"
#include <cstring>

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
#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%.17g")
#define EXPECT_EQ_STRING(expect, actual, alength) \
    EXPECT_EQ_BASE(sizeof(expect) - 1 == alength && memcmp(expect, actual, alength) == 0, expect, actual, "%s")

static ExceptType lept_parse(Value *&v, char *a) {
    try {
        v = Parser::parse(a);
        return PARSE_OK;
    } catch (ExceptType e) {
        return e;
    }
}

static void test_parse_null() {
    Value *v;
    EXPECT_EQ_INT(PARSE_OK, lept_parse(v, "null"));
    EXPECT_EQ_INT(VALUE_NULL, v->get_type());
    Value::delete_value(v);
}

static void test_parse_true() {
    Value *v;
    EXPECT_EQ_INT(PARSE_OK, lept_parse(v, "true"));
    EXPECT_EQ_INT(VALUE_TRUE, v->get_type());
    Value::delete_value(v);
}

static void test_parse_false() {
    Value *v;
    EXPECT_EQ_INT(PARSE_OK, lept_parse(v, "false"));
    EXPECT_EQ_INT(VALUE_FALSE, v->get_type());
    Value::delete_value(v);
}

#define TEST_NUMBER(expect, json)\
    do {\
        Value *v;\
        EXPECT_EQ_INT(PARSE_OK, lept_parse(v, json));\
        EXPECT_EQ_INT(VALUE_NUMBER, v->get_type());\
        EXPECT_EQ_DOUBLE(expect, v->get_num());\
        Value::delete_value(v);\
    } while(0)

static void test_parse_number() {
    TEST_NUMBER(0.0, "0");
    TEST_NUMBER(0.0, "-0");
    TEST_NUMBER(0.0, "-0.0");
    TEST_NUMBER(1.0, "1");
    TEST_NUMBER(-1.0, "-1");
    TEST_NUMBER(1.5, "1.5");
    TEST_NUMBER(-1.5, "-1.5");
    TEST_NUMBER(3.1416, "3.1416");
    TEST_NUMBER(1E10, "1E10");
    TEST_NUMBER(1e10, "1e10");
    TEST_NUMBER(1E+10, "1E+10");
    TEST_NUMBER(1E-10, "1E-10");
    TEST_NUMBER(-1E10, "-1E10");
    TEST_NUMBER(-1e10, "-1e10");
    TEST_NUMBER(-1E+10, "-1E+10");
    TEST_NUMBER(-1E-10, "-1E-10");
    TEST_NUMBER(1.234E+10, "1.234E+10");
    TEST_NUMBER(1.234E-10, "1.234E-10");
    TEST_NUMBER(0.0, "1e-10000"); /* must underflow */

    TEST_NUMBER(1.0000000000000002, "1.0000000000000002"); /* the smallest number > 1 */
    TEST_NUMBER( 4.9406564584124654e-324, "4.9406564584124654e-324"); /* minimum denormal */
    TEST_NUMBER(-4.9406564584124654e-324, "-4.9406564584124654e-324");
    TEST_NUMBER( 2.2250738585072009e-308, "2.2250738585072009e-308");  /* Max subnormal double */
    TEST_NUMBER(-2.2250738585072009e-308, "-2.2250738585072009e-308");
    TEST_NUMBER( 2.2250738585072014e-308, "2.2250738585072014e-308");  /* Min normal positive double */
    TEST_NUMBER(-2.2250738585072014e-308, "-2.2250738585072014e-308");
    TEST_NUMBER( 1.7976931348623157e+308, "1.7976931348623157e+308");  /* Max double */
    TEST_NUMBER(-1.7976931348623157e+308, "-1.7976931348623157e+308");
}

#define TEST_STRING(expect, json)\
    do {\
        Value *v;\
        EXPECT_EQ_INT(PARSE_OK, lept_parse(v, json));\
        EXPECT_EQ_INT(VALUE_STRING, v->get_type());\
        EXPECT_EQ_STRING(expect, v->get_str().c_str(), v->get_str().size());\
        Value::delete_value(v);\
    } while(0)

static void test_parse_string() {
    TEST_STRING("", "\"\"");
    TEST_STRING("Hello", "\"Hello\"");
    TEST_STRING("Hello world", "\"Hello world\"");
}

#define TEST_ERROR(error, json)\
    do {\
        Value *v;\
        EXPECT_EQ_INT(error, lept_parse(v, json));\
    } while(0)

static void test_parse_expect_value() {
    TEST_ERROR(PARSE_EXPECT_VALUE, "");
    TEST_ERROR(PARSE_EXPECT_VALUE, " ");
}

static void test_parse_invalid_value() {
    TEST_ERROR(PARSE_INVALID_VALUE, "nul");
    TEST_ERROR(PARSE_INVALID_VALUE, "?");

    /* invalid number */
    TEST_ERROR(PARSE_INVALID_VALUE, "+0");
    TEST_ERROR(PARSE_INVALID_VALUE, "+1");
    TEST_ERROR(PARSE_INVALID_VALUE, ".123"); /* at least one digit before '.' */
    TEST_ERROR(PARSE_INVALID_VALUE, "1.");   /* at least one digit after '.' */
    TEST_ERROR(PARSE_INVALID_VALUE, "INF");
    TEST_ERROR(PARSE_INVALID_VALUE, "inf");
    TEST_ERROR(PARSE_INVALID_VALUE, "NAN");
    TEST_ERROR(PARSE_INVALID_VALUE, "nan");
}

static void test_parse_root_not_singular() {
    TEST_ERROR(PARSE_ROOT_NOT_SINGULAR, "null x");

    /* invalid number */
    TEST_ERROR(PARSE_ROOT_NOT_SINGULAR, "0123"); /* after zero should be '.' or nothing */
    TEST_ERROR(PARSE_ROOT_NOT_SINGULAR, "0x0");
    TEST_ERROR(PARSE_ROOT_NOT_SINGULAR, "0x123");
}

static void test_parse_number_too_big() {
    TEST_ERROR(PARSE_NUMBER_TOO_BIG, "1e309");
    TEST_ERROR(PARSE_NUMBER_TOO_BIG, "-1e309");
}

static void test_parse_missing_quotation_mark() {
    TEST_ERROR(PARSE_MISS_QUOTATION_MARK, "\"");
    TEST_ERROR(PARSE_MISS_QUOTATION_MARK, "\"abc");
}


static void test_access_type() {
    Value v;
    v.set_type(VALUE_NULL);
    EXPECT_EQ_INT(VALUE_NULL, v.get_type());
    v.set_type(VALUE_TRUE);
    EXPECT_EQ_INT(VALUE_TRUE, v.get_type());
}

static void test_access_number() {
    Value v;
    v.set_num(1234.5);
    EXPECT_EQ_DOUBLE(1234.5, v.get_num());
}

static void test_access_string() {
    Value v;
    v.set_str("");
    EXPECT_EQ_STRING("", v.get_str().c_str(), v.get_str().size());
    v.set_str("Hello");
    EXPECT_EQ_STRING("Hello", v.get_str().c_str(), v.get_str().size());
}

static void test_parse() {
    test_parse_null();
    test_parse_true();
    test_parse_false();
    test_parse_number();
    test_parse_string();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
    test_parse_number_too_big();
    test_parse_missing_quotation_mark();

    test_access_type();
    test_access_number();
    test_access_string();
}

int main() {
    test_parse();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
    return EXIT_SUCCESS;
}