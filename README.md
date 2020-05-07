<meta name="referrer" content="no-referrer" />

## 从零开始的JSON库

项目地址 [jsoncpp](https://github.com/zzzz76/jsoncpp) ，此项目受到 [leptjson](https://github.com/miloyip) 启发，实现了最基本的功能，仅作学习使用。

 * 提供简单的 parse() 和 generate() 方法将 JSON 文本解析成对象，反之亦然
 * 仅支持将基本类型为 Value 的 JSON 抽象语法树与 JSON 文本相互转换
 * 使用标准 C/C++ 语言 (C++11)
 * 不依赖第三方库

<a href="https://img2020.cnblogs.com/blog/1039974/202004/1039974-20200403222129004-694085553.png" referrer="no-referrer|origin|unsafe-url"><img src="https://img2020.cnblogs.com/blog/1039974/202004/1039974-20200403222129004-694085553.png" width="70%" height="70%"></a>

## 启动

启动此项目需要安装 gcc4.8 以上版本（支持c++11即可）

~~~shell
rm *.o
rm start
g++ -w -c -std=c++11 jsonValue.cpp
g++ -w -c -std=c++11 jsonGenerator.cpp
g++ -w -c -std=c++11 jsonParser.cpp
g++ -w -c -std=c++11 test.cpp
g++ -w jsonValue.o jsonGenerator.o jsonParser.o test.o -o start
./start
~~~

## 初步入门

如何解析一段json文本呢，我们可以先从更简单的问题开始思考。

##### 1、给定长文本 "[1,2,3]" ，问如何才能得到一个数组？

当我们想通过解析函数的连续调用去处理一段长文本时，解析函数需要分两步实现：

* step1 解析文本并更新文本指针。
* step2 若解析成功则返回解析结果。

这里只需要调用三次解析函数，最后遇到 ']' 结束即可

~~~c++
class Parser{
    char *txt;
    int parse_value() {
        /* 解析txt上下文 */
        /* 返回一个数值 */
    }
}

int main() {
    /* 建立parser */
    while (*parser.txt != ']') {
        array[i] = parser.parse_value();
    }
}
~~~

##### 2、给定长文本 "[1,2,[4,5]]" ，问如何才能得到一棵树?

同理，这里需要调用三次解析函数，最后遇到 ']' 结束即可

~~~c++
typedef struct {
    int number;
    vector<Node* > array;
} TreeNode

class Parser{
    char* txt;
    TreeNode* parse_value() {
        /* 解析txt上下文 */
        /* 返回一个节点 */
    }
}

int main() {
    /* 建立parser */
    /* 建立树根 */
    while (*parser.txt != ']') {
        root.array[i] = parser.parse_value();
    }
}
~~~

完成了长文本的解析之后，再考虑解析函数本身的实现。 '1' , '2' 可以直接解析，而 "[4,5]" 可以看做一个长文本，做递归调用即可。

以上过程我均采用了面向对象的设计，每一个函数都是对对象的操作。当然，大家也可以采用面向过程的思路，但是务必将 txt 文本指针的地址传入每个函数中，或者采用引用。

##### 3、给定长文本 JSON ，问如何才能得到一棵树？

本项目 [jsoncpp](https://github.com/zzzz76/jsoncpp) 便是此问题的答案。

其中 parse 和 generate 方法解析时序图：
<a href="https://img2020.cnblogs.com/blog/1039974/202004/1039974-20200420224035900-1326683849.png" referrer="no-referrer|origin|unsafe-url"><img src="https://img2020.cnblogs.com/blog/1039974/202004/1039974-20200420224035900-1326683849.png" width="70%" height="70%"></a>