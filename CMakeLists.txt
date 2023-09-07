cmake_minimum_required(VERSION 3.10)

project("A_star")

file (GLOB SOURCES src/*.c)
file (GLOB HEADERS src/*.h)

set (CMAKE_C_FLAGS "-Wall -g -ftrapv")

find_package(SDL2 REQUIRED)
add_executable(${PROJECT_NAME} main.c ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
