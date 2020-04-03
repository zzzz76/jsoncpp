/**
 * Created with CLion
 * User: zzzz76
 * Date: 2020-04-02
 */

#include "jsonGenerator.h"

// 生成null文本
void Generator::generate_null() {
    txt += "null";
}

// 生成false文本
void Generator::generate_false() {
    txt += "false";
}

// 生成true文本
void Generator::generate_true() {
    txt += "true";
}

// 生成number文本
void Generator::generate_number(Value *v) {
//    txt += to_string(v->get_num());
    char buffer[32];
    int length = sprintf(buffer, "%.17g", v->get_num());
    txt.append(buffer, length);
}

// 生成string文本
void Generator::generate_string(Value *v) {
    txt.push_back('"');
    txt += v->get_str();
    txt.push_back('"');
}

// 生成array文本
void Generator::generate_array(Value *v) {
    txt.push_back('[');
    for (auto &it : v->get_array()) {
        generate_value(it);
        txt.push_back(',');
    }
    if (!v->get_array().empty()) {
        txt.pop_back();
    }
    txt.push_back(']');
}

// 生成object文本
void Generator::generate_object(Value *v) {
    // 先进行遍历
    txt.push_back('{');
    for (auto &it : v->get_object()) {
        txt.push_back('"');
        txt += it.first;
        txt.push_back('"');
        txt.push_back(':');
        generate_value(it.second);
        txt.push_back(',');
    }
    if (!v->get_object().empty()) {
        txt.pop_back();
    }
    txt.push_back('}');
}

// 生成value文本
void Generator::generate_value(Value *v) {
    switch (v->get_type()) {
        case VALUE_NULL:
            return generate_null();
        case VALUE_FALSE:
            return generate_false();
        case VALUE_TRUE:
            return generate_true();
        case VALUE_NUMBER:
            return generate_number(v);
        case VALUE_STRING:
            return generate_string(v);
        case VALUE_ARRAY:
            return generate_array(v);
        case VALUE_OBJECT:
            return generate_object(v);
    }
}

string Generator::generate(Value *v) {
    assert(v != NULL);
    Generator generator;
    generator.generate_value(v);
    return generator.txt;
}
