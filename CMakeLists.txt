cmake_minimum_required(VERSION 3.7)
project(furo)

set(CMAKE_CXX_STANDARD 11)
set(furo_version 1.0.0)
include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB src "src/*.c")
file(GLOB tests "tests/*.c")

add_library(rivulet ${src})

add_executable(rivulet_test ${tests})
target_link_libraries(rivulet_test LINK_PUBLIC furo)
