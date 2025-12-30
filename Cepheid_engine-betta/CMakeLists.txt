cmake_minimum_required(VERSION 3.5)

project(Cepheid_engie_betta)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG -mwindows")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")

add_executable( ${PROJECT_NAME}  "main.cpp")

add_subdirectory( ${PROJECT_SOURCE_DIR}/lib/CepheidEngine)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/lib/CepheidEngine/include)

add_compile_options(-fPIC -march=haswell)

target_link_libraries(${PROJECT_NAME} CepheidEngine-static)

add_subdirectory( ${PROJECT_SOURCE_DIR}/DLC)
