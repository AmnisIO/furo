cmake_minimum_required(VERSION 3.7)
project(rivulet)

set(CMAKE_CXX_STANDARD 11)
set(furo_version 1.0.0)
include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB src "src/*.c")
file(GLOB test "test/*.c")

add_library(rivulet ${src})

add_executable(rivulet_test ${test})
target_link_libraries(rivulet_test LINK_PUBLIC rivulet)
