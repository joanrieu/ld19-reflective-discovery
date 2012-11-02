cmake_minimum_required(VERSION 2.8)
project(DISCOVERY)

add_definitions(-Wall -Wextra -ggdb -O0 "\"-DDEBUGTRAP()=asm(\\\"int3;nop\\\")\"")

add_subdirectory(src bin)
