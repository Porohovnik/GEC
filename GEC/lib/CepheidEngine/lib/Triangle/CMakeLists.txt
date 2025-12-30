cmake_minimum_required(VERSION 3.13)

project(Triangle)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)


#=======================================================================================================================
# PROJECT FILES
#   List of project source files
#=======================================================================================================================
file(GLOB_RECURSE PUBLIC_HEADERS_FILES  RELATIVE ${PROJECT_SOURCE_DIR} "*.h")
file(GLOB_RECURSE SOURCE_LIB            RELATIVE ${PROJECT_SOURCE_DIR} "*.c")

add_executable( ${PROJECT_NAME}  "main.c" "triangle.c")

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})


