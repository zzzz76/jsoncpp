#include "jsonValue.h"

void Value::set_array(vector<Value *> array) {
    this->array = array;
}

void Value::set_maptable(map<string, Value *> maptable) {
    this->maptable = maptable;
}

void Value::set_str(string str) {
    this->str = str;
}


void Value::set_num(double num) {
    this->num = num;
}

void Value::set_type(ValueType type) {
    this->type = type;
}

vector<Value *> Value::get_array() {
    return array;
}

map<string, Value *> Value::get_maptable() {
    return maptable;
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
    if (v->type == VALUE_ARRAY) {
        for (auto &it : v->array) {
            delete_value(it);
        }
    }
    if (v->type == VALUE_OBJECT) {
        for (auto &it : v->maptable) {
            delete_value(it.second);
        }
    }
    delete (v);
    v = nullptr;
}
