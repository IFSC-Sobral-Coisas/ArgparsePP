cmake_minimum_required(VERSION 3.16)
project(argparse_demo)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)
add_executable(demo main.cpp  Argparse.cpp Argparse.h teste.cpp)
add_executable(teste teste.cpp)
