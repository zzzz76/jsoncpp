cmake_minimum_required(VERSION 3.5)
project(jsoncpp)

set(CMAKE_CXX_STANDARD 11)

add_executable(jsoncpp src/jsonParser.cpp src/jsonParser.h test.cpp leptjson.cpp leptjson.h)