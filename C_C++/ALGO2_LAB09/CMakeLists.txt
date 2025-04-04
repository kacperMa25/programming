cmake_minimum_required(VERSION 3.30)
project(LAB09)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp)

add_custom_target(run_with_args
    COMMAND ${PROJECT_NAME} < "../../excercises/points1.txt"
    DEPENDS command_line_args
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
