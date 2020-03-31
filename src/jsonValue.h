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
    map<string, Value *> maptable;
    string str;
    double num;
    ValueType type;
public:
    void set_array(vector<Value *> array);
    void set_maptable(map<string, Value *> maptable);
    void set_str(string str);
    void set_num(double num);
    void set_type(ValueType type);

    vector<Value *> get_array();
    map<string, Value *> get_maptable();
    string get_str();
    double get_num();
    ValueType get_type();

};


#endif //JSONCPP_JSONVALUE_H
