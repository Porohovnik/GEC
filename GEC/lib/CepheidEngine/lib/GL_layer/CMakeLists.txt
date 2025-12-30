cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(gl_layer)

#=======================================================================================================================
# Output Directories.
#=======================================================================================================================
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

#=======================================================================================================================
# Add build options
#=======================================================================================================================
option(GL_LAYER_BUILD_OBJECT_LIB "Build object library (not available in Xcode)" OFF)
option(GL_LAYER_BUILD_STATIC_LIB "Build static library                         " ON)
option(GL_LAYER_BUILD_SHARED_LIB "Build shared library                         " OFF)

if(DEFINED BUILD_SHARED_LIBS)
    set(MYLIB_SHARED_LIBS ${BUILD_SHARED_LIBS})
endif()

#=======================================================================================================================
# PROJECT FILES
#   List of project source files
#=======================================================================================================================
file(GLOB_RECURSE PUBLIC_HEADERS_FILES  RELATIVE ${PROJECT_SOURCE_DIR} "include/*.h")
file(GLOB_RECURSE SOURCE_LIB            RELATIVE ${PROJECT_SOURCE_DIR} "src/*.cpp")

#find_package(OpenGL REQUIRED)
#=======================================================================================================================
# LIBS CHOOSING TYPES
#=======================================================================================================================
if (${GL_LAYER_BUILD_OBJECT_LIB})
    list(APPEND LIBRARY_TYPE_NAME  ${PROJECT_NAME}-object)
    add_library(${PROJECT_NAME}-object OBJECT)
endif()

if (${GL_LAYER_BUILD_STATIC_LIB})
    list(APPEND LIBRARY_TYPE_NAME  ${PROJECT_NAME}-static)
    add_library(${PROJECT_NAME}-static STATIC)
endif()

if (${GL_LAYER_BUILD_SHARED_LIB})
    list(APPEND LIBRARY_TYPE_NAME  ${PROJECT_NAME}-shared)
    add_library(${PROJECT_NAME}-shared SHARED)
endif()
#=======================================================================================================================
# DEFINE LIBS
#=======================================================================================================================
add_subdirectory(${PROJECT_SOURCE_DIR}/contrib/glew/build/cmake)

#=======================================================================================================================
# DEFINE FILE
#=======================================================================================================================
foreach(NAME IN LISTS LIBRARY_TYPE_NAME)
    target_sources(${NAME} PRIVATE ${PUBLIC_HEADERS_FILES} ${SOURCE_LIB})
    target_include_directories(${NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

    target_include_directories(${NAME} PRIVATE ${PROJECT_SOURCE_DIR}/contrib/glew/include)
    target_include_directories(${NAME} PRIVATE ${PROJECT_SOURCE_DIR}/contrib/Tuple-utilities-template-library)
    target_link_libraries(${NAME} PRIVATE glew_s)
endforeach()

