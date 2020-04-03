cmake_minimum_required(VERSION 3.5)
project(jsoncpp)

set(CMAKE_CXX_STANDARD 11)

add_executable(jsoncpp jsonParser.cpp jsonParser.h test.cpp jsonValue.cpp jsonValue.h jsonGenerator.cpp jsonGenerator.h)