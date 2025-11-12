cmake_minimum_required(VERSION 3.15)
project(nixjson VERSION 1.0 LANGUAGES C)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_library(nixjson STATIC ${SOURCES})

target_include_directories(nixjson PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

add_executable(nixjson_example examples/example1.c)
target_link_libraries(nixjson_example PRIVATE nixjson)