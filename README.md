## json 是什么

ValueObject 是一个数组对象，数组含有同一种元素
~~~
struct ValueObject {
    Element list[10];
}
~~~

Element 是一个对象，由key和value组成
~~~
struct Element {
    string elem_key;
    ElemValue elem_value;
}
~~~

ElemValue 是一个对象，其有六中形式 
~~~
ElemValue == null
ElemValue == boolean
ElemValue == Number
ElemValue == string
ElemValue == ValueArray
ElemValue == ValueObject
~~~

ValueArray 是一个数组对象，数组含有同一种元素
~~~
struct ValueArray {
    ElemValue list[10];
}
~~~

json字符串样例
~~~
{
    "title": "Design Patterns",
    "subtitle": "Elements of Reusable Object-Oriented Software",
    "author": [
        "Erich Gamma",
        "Richard Helm",
        "Ralph Johnson",
        "John Vlissides"
    ],
    "year": 2009,
    "weight": 1.8,
    "hardcover": true,
    "publisher": {
        "Company": "Pearson Education",
        "Country": "India"
    },
    "website": null
}
~~~


## 面向对象

java：基本类型的传参均为复制
swap(int a[], int index1, int index2)

Node node = new Node();

c++：基本类型的传参均为复制
注明：c++中结构体亦为基本类型
swap(int* a, int index1, int index2)

Node *node = new Node();

## 函数异常

任何函数均有其使用规范
比如：除法、下标访问、数值转换

当正确的使用函数时，函数可以支持
当错误的使用函数时，函数无法支持

对于第二种情况，需要立刻终止函数的执行
比如：编译错误、程序中断(溢出、报错)

