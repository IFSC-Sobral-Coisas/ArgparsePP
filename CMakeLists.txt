cmake_minimum_required(VERSION 3.16)
project(Argparser)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)
add_library(argparse Argparse.cpp)
#add_executable(demo demos/demo.cpp)
