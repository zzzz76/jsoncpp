#include "src/jsonParser.h"
#include "src/jsonGenerator.h"
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
#if defined(_MSC_VER)
#define EXPECT_EQ_SIZE_T(expect, actual) EXPECT_EQ_BASE((expect) == (actual), (size_t)expect, (size_t)actual, "%Iu")
#else
#define EXPECT_EQ_SIZE_T(expect, actual) EXPECT_EQ_BASE((expect) == (actual), (size_t)expect, (size_t)actual, "%zu")
#endif

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
    TEST_STRING("Hello\nworld", "\"Hello\nworld\"");
    TEST_STRING("Hello\\nworld", "\"Hello\\nworld\"");
}

static void test_parse_array() {
    Value *v;
    EXPECT_EQ_INT(PARSE_OK, lept_parse(v, "[ ]"));
    EXPECT_EQ_INT(VALUE_ARRAY, v->get_type());
    EXPECT_EQ_SIZE_T(0, v->get_array().size());
    Value::delete_value(v);


    EXPECT_EQ_INT(PARSE_OK, lept_parse(v, "[ null , false , true , 123 , \"abc\" ]"));
    EXPECT_EQ_INT(VALUE_ARRAY, v->get_type());
    EXPECT_EQ_SIZE_T(5, v->get_array().size());
    EXPECT_EQ_INT(VALUE_NULL,   v->get_array()[0]->get_type());
    EXPECT_EQ_INT(VALUE_FALSE,  v->get_array()[1]->get_type());
    EXPECT_EQ_INT(VALUE_TRUE,   v->get_array()[2]->get_type());
    EXPECT_EQ_INT(VALUE_NUMBER, v->get_array()[3]->get_type());
    EXPECT_EQ_INT(VALUE_STRING, v->get_array()[4]->get_type());
    EXPECT_EQ_DOUBLE(123.0, v->get_array()[3]->get_num());
    EXPECT_EQ_STRING("abc", v->get_array()[4]->get_str().c_str(), v->get_array()[4]->get_str().size());
    Value::delete_value(v);

    EXPECT_EQ_INT(PARSE_OK, lept_parse(v, "[ [ ] , [ 0 ] , [ 0 , 1 ] , [ 0 , 1 , 2 ] ]"));
    EXPECT_EQ_INT(VALUE_ARRAY, v->get_type());
    EXPECT_EQ_SIZE_T(4, v->get_array().size());
    for (int i = 0; i < 4; i++) {
        Value* a = v->get_array()[i];
        EXPECT_EQ_INT(VALUE_ARRAY, a->get_type());
        EXPECT_EQ_SIZE_T(i, a->get_array().size());
        for (int j = 0; j < i; j++) {
            Value* e = a->get_array()[j];
            EXPECT_EQ_INT(VALUE_NUMBER, e->get_type());
            EXPECT_EQ_DOUBLE((double)j, e->get_num());
        }
    }
    Value::delete_value(v);
}

static void test_parse_object() {
    Value *v;

    EXPECT_EQ_INT(PARSE_OK, lept_parse(v, " { } "));
    EXPECT_EQ_INT(VALUE_OBJECT, v->get_type());
    EXPECT_EQ_SIZE_T(0, v->get_object().size());
    Value::delete_value(v);

    EXPECT_EQ_INT(PARSE_OK, lept_parse(v,
                                            " { "
                                                    "\"n\" : null , "
                                                    "\"f\" : false , "
                                                    "\"t\" : true , "
                                                    "\"i\" : 123 , "
                                                    "\"s\" : \"abc\", "
                                                    "\"a\" : [ 1, 2, 3 ],"
                                                    "\"o\" : { \"1\" : 1, \"2\" : 2, \"3\" : 3 }"
                                                    " } "
    ));
    EXPECT_EQ_INT(VALUE_OBJECT, v->get_type());
    EXPECT_EQ_SIZE_T(7, v->get_object().size());
    EXPECT_EQ_INT(VALUE_NULL,   v->get_object()["n"]->get_type());
    EXPECT_EQ_INT(VALUE_FALSE,  v->get_object()["f"]->get_type());
    EXPECT_EQ_INT(VALUE_TRUE,   v->get_object()["t"]->get_type());
    EXPECT_EQ_INT(VALUE_NUMBER, v->get_object()["i"]->get_type());
    EXPECT_EQ_DOUBLE(123.0, v->get_object()["i"]->get_num());
    EXPECT_EQ_INT(VALUE_STRING, v->get_object()["s"]->get_type());
    EXPECT_EQ_STRING("abc", v->get_object()["s"]->get_str().c_str(), v->get_object()["s"]->get_str().size());
    EXPECT_EQ_INT(VALUE_ARRAY, v->get_object()["a"]->get_type());
    EXPECT_EQ_SIZE_T(3, v->get_object()["a"]->get_array().size());
    for (int i = 0; i < 3; i++) {
        Value *e = v->get_object()["a"]->get_array()[i];
        EXPECT_EQ_INT(VALUE_NUMBER, e->get_type());
        EXPECT_EQ_DOUBLE(i + 1.0, e->get_num());
    }
    Value* o = v->get_object()["o"];
    EXPECT_EQ_INT(VALUE_OBJECT, o->get_type());
    EXPECT_EQ_SIZE_T(3, o->get_object().size());
    for (int i = 0; i < 3; i++) {
        Value* ov = o->get_object()[to_string(i + 1)];
        EXPECT_EQ_INT(VALUE_NUMBER, ov->get_type());
        EXPECT_EQ_DOUBLE(i + 1.0, ov->get_num());
    }
    Value::delete_value(v);
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

    /* invalid value in array */
    TEST_ERROR(PARSE_INVALID_VALUE, "[1,]");
    TEST_ERROR(PARSE_INVALID_VALUE, "[\"a\", nul]");
}

static void test_parse_root_not_singular() {
    TEST_ERROR(PARSE_ROOT_NOT_SINGULAR, "null x");

    /* invalid number */
    TEST_ERROR(PARSE_ROOT_NOT_SINGULAR, "0123"); /* after zero should be '.' or nothing */
    TEST_ERROR(PARSE_ROOT_NOT_SINGULAR, "0x0");
    TEST_ERROR(PARSE_ROOT_NOT_SINGULAR, "0x123");
}

static void test_parse_missing_quotation_mark() {
    TEST_ERROR(PARSE_MISS_QUOTATION_MARK, "\"");
    TEST_ERROR(PARSE_MISS_QUOTATION_MARK, "\"abc");
}

static void test_parse_miss_comma_or_square_bracket() {
    TEST_ERROR(PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[1");
    TEST_ERROR(PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[1}");
    TEST_ERROR(PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[1 2");
    TEST_ERROR(PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[[]");
}

static void test_parse_miss_key() {
    TEST_ERROR(PARSE_MISS_KEY, "{:1,");
    TEST_ERROR(PARSE_MISS_KEY, "{1:1,");
    TEST_ERROR(PARSE_MISS_KEY, "{true:1,");
    TEST_ERROR(PARSE_MISS_KEY, "{false:1,");
    TEST_ERROR(PARSE_MISS_KEY, "{null:1,");
    TEST_ERROR(PARSE_MISS_KEY, "{[]:1,");
    TEST_ERROR(PARSE_MISS_KEY, "{{}:1,");
    TEST_ERROR(PARSE_MISS_KEY, "{\"a\":1,");
}

static void test_parse_miss_colon() {
    TEST_ERROR(PARSE_MISS_COLON, "{\"a\"}");
    TEST_ERROR(PARSE_MISS_COLON, "{\"a\",\"b\"}");
}

static void test_parse_miss_comma_or_curly_bracket() {
    TEST_ERROR(PARSE_MISS_COMMA_OR_CURLY_BRACKET, "{\"a\":1");
    TEST_ERROR(PARSE_MISS_COMMA_OR_CURLY_BRACKET, "{\"a\":1]");
    TEST_ERROR(PARSE_MISS_COMMA_OR_CURLY_BRACKET, "{\"a\":1 \"b\"");
    TEST_ERROR(PARSE_MISS_COMMA_OR_CURLY_BRACKET, "{\"a\":{}");
}



static void test_parse() {
    test_parse_null();
    test_parse_true();
    test_parse_false();
    test_parse_number();
    test_parse_string();
    test_parse_array();
    test_parse_object();

    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
    test_parse_missing_quotation_mark();
    test_parse_miss_comma_or_square_bracket();
    test_parse_miss_key();
    test_parse_miss_colon();
    test_parse_miss_comma_or_curly_bracket();
}

#define TEST_ROUNDTRIP(json)\
    do {\
        Value *v;\
        EXPECT_EQ_INT(PARSE_OK, lept_parse(v, json));\
        string str = Generator::generate(v);\
        EXPECT_EQ_STRING(json, str.c_str(), str.size());\
        Value::delete_value(v);\
    } while(0)

static void test_generate_number() {
    TEST_ROUNDTRIP("0");
    TEST_ROUNDTRIP("-0");
    TEST_ROUNDTRIP("1");
    TEST_ROUNDTRIP("-1");
    TEST_ROUNDTRIP("1.5");
    TEST_ROUNDTRIP("-1.5");
    TEST_ROUNDTRIP("3.25");
    TEST_ROUNDTRIP("1e+20");
    TEST_ROUNDTRIP("1.234e+20");
    TEST_ROUNDTRIP("1.234e-20");

    TEST_ROUNDTRIP("1.0000000000000002"); /* the smallest number > 1 */
    TEST_ROUNDTRIP("4.9406564584124654e-324"); /* minimum denormal */
    TEST_ROUNDTRIP("-4.9406564584124654e-324");
    TEST_ROUNDTRIP("2.2250738585072009e-308");  /* Max subnormal double */
    TEST_ROUNDTRIP("-2.2250738585072009e-308");
    TEST_ROUNDTRIP("2.2250738585072014e-308");  /* Min normal positive double */
    TEST_ROUNDTRIP("-2.2250738585072014e-308");
    TEST_ROUNDTRIP("1.7976931348623157e+308");  /* Max double */
    TEST_ROUNDTRIP("-1.7976931348623157e+308");
}

static void test_generate_string() {
    TEST_ROUNDTRIP("\"\"");
    TEST_ROUNDTRIP("\"Hello\"");
    TEST_ROUNDTRIP("\"Hello\\nWorld\"");
}

static void test_generate_array() {
    TEST_ROUNDTRIP("[]");
    TEST_ROUNDTRIP("[null,false,true,123,\"abc\",[1,2,3]]");
}

static void test_generate_object() {
    TEST_ROUNDTRIP("{}");
    TEST_ROUNDTRIP("{\"a\":[1,2,3],\"f\":false,\"i\":123,\"n\":null,\"o\":{\"1\":1,\"2\":2,\"3\":3},\"s\":\"abc\",\"t\":true}");
}

static void test_generate() {
    TEST_ROUNDTRIP("null");
    TEST_ROUNDTRIP("false");
    TEST_ROUNDTRIP("true");
    test_generate_number();
    test_generate_string();
    test_generate_array();
    test_generate_object();
}

static void test_access_type() {
    Value *v = new Value;
    EXPECT_EQ_INT(VALUE_NULL, v->get_type());
    Value::delete_value(v);
    v = new Value(true);
    EXPECT_EQ_INT(VALUE_TRUE, v->get_type());
    Value::delete_value(v);
}

static void test_access_number() {
    Value *v = new Value(1234.5);
    EXPECT_EQ_DOUBLE(1234.5, v->get_num());
    Value::delete_value(v);
}

static void test_access_string() {
    Value *v = new Value(string(""));
    EXPECT_EQ_STRING("", v->get_str().c_str(), v->get_str().size());
    Value::delete_value(v);
    v = new Value(string("Hello"));
    EXPECT_EQ_STRING("Hello", v->get_str().c_str(), v->get_str().size());
    Value::delete_value(v);
}

static void test_access() {
    test_access_type();
    test_access_number();
    test_access_string();
}

int main() {
    test_parse();
    test_generate();
    test_access();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
    return EXIT_SUCCESS;
}