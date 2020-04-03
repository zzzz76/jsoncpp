/**
 * Created with CLion
 * User: zzzz76
 * Date: 2020-04-02
 */

#ifndef JSONCPP_JSONGENERATOR_H
#define JSONCPP_JSONGENERATOR_H

#include "jsonValue.h"

class Generator {
private:
    string txt;

    void generate_null();
    void generate_false();
    void generate_true();
    void generate_number(Value *v);
    void generate_string(Value *v);
    void generate_array(Value *v);
    void generate_object(Value *v);
    void generate_value(Value *v);

public:
    static string generate(Value *v);
};


#endif //JSONCPP_JSONGENERATOR_H
