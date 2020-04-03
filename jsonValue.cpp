#include "jsonValue.h"

Value::Value(vector<Value *> array) {
    this->type = VALUE_ARRAY;
    this->array = array;
}

Value::Value(map<string, Value *> object) {
    this->type = VALUE_OBJECT;
    this->object = object;
}

Value::Value(string str) {
    this->type = VALUE_STRING;
    this->str = str;
}

Value::Value(double num) {
    this->type = VALUE_NUMBER;
    this->num = num;
}

Value::Value(bool type) {
    if (type) this->type = VALUE_TRUE;
    else this->type = VALUE_FALSE;
}

Value::Value() {
    this->type = VALUE_NULL;
}

vector<Value *> Value::get_array() {
    return array;
}

map<string, Value *> Value::get_object() {
    return object;
}

string Value::get_str() {
    return str;
}

double Value::get_num() {
    return num;
}

ValueType Value::get_type() {
    return type;
}

// 此函数可以释放当前对象
void Value::delete_value(Value *&v) {
    // 对value的释放可以进行分解
    assert(v != NULL);
    if (v->type == VALUE_ARRAY) {
        for (auto &it : v->array) {
            delete_value(it);
        }
    }
    if (v->type == VALUE_OBJECT) {
        for (auto &it : v->object) {
            delete_value(it.second);
        }
    }
    delete (v);
    v = NULL;
}
