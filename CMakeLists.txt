cmake_minimum_required(VERSION 3.5)
project(jsoncpp)

set(CMAKE_CXX_STANDARD 11)

add_executable(jsoncpp jsonc.cpp jsonc.h test.cpp)