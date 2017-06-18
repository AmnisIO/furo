cmake_minimum_required(VERSION 3.7)
project(furo)

set(CMAKE_CXX_STANDARD 11)
set(furo_version 1.0.0)
include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB src "src/*.c")
file(GLOB tests "tests/*.c")

include(GenerateExportHeader)

add_library(furo ${src})

add_executable(furo_test ${tests})
target_link_libraries(furo_test LINK_PUBLIC furo)
