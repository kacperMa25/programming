CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
PROJECT(SDL3Project VERSION 0.1.0 LANGUAGES C CXX)

SET(CMAKE_CXX STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
OPTION(BUILD_SHARED_LIBS "Build using shared library" ON)

FIND_PACKAGE(SDL3 REQUIRED)
add_executable(${PROJECT_NAME} main.cpp)

# Set include directories
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Link SDL3
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
