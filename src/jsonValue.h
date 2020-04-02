#ifndef JSONCPP_JSONVALUE_H
#define JSONCPP_JSONVALUE_H

#include <vector>
#include <string>
#include <map>

using namespace std;

typedef enum {
    VALUE_NULL, VALUE_FALSE, VALUE_TRUE, VALUE_NUMBER, VALUE_STRING, VALUE_ARRAY, VALUE_OBJECT
} ValueType;

class Value {
private:
    vector<Value *> array;
    map<string, Value *> object;
    string str;
    double num;
    ValueType type;
public:
    Value(vector<Value *> array);
    Value(map<string, Value *> object);
    Value(string str);
    Value(double num);
    Value(bool type);
    Value();

    vector<Value *> get_array();
    map<string, Value *> get_object();
    string get_str();
    double get_num();
    ValueType get_type();

    static void delete_value(Value *&v);
};


#endif //JSONCPP_JSONVALUE_H
