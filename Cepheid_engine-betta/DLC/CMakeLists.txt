cmake_minimum_required( VERSION 3.4.0 )
# Если версия установленой программы ниже, то ошибка выполнения

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(shared_lib)

set(NAME_DLC
     "base"
     "kadastr"
     "GEC"
  )

if(WIN32)
    set(OPENSSL_ROOT_DIR          ${PROJECT_SOURCE_DIR}/lib/openssl-cmake-master)
    set(OPENSSL_INCLUDE_DIR       ${PROJECT_SOURCE_DIR}/lib/openssl-cmake-master/include)
    set(OPENSSL_LIBRARIES         ${PROJECT_SOURCE_DIR}/lib/openssl-cmake-master/lib)
    set ( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )
endif (WIN32)

add_subdirectory( ${PROJECT_SOURCE_DIR}/lib/OpenXLSX-master)
add_subdirectory( ${PROJECT_SOURCE_DIR}/lib/libsqlite3-master)
add_subdirectory( ${PROJECT_SOURCE_DIR}/lib/csv-parser-master)
add_subdirectory( ${PROJECT_SOURCE_DIR}/lib/curl-master)

target_compile_options(libcurl PUBLIC  -fPIC -march=haswell)

foreach(X IN LISTS NAME_DLC)
    file(GLOB SOURCES
        "${PROJECT_SOURCE_DIR}/${X}/*.cpp"
        "${PROJECT_SOURCE_DIR}/${X}/*.h"
        )
    set(SOURCE_LIB ${SOURCES})
    add_compile_options(-fPIC -march=haswell)

    add_library(${X} SHARED ${SOURCE_LIB})
    target_include_directories(${X} PRIVATE ${PROJECT_SOURCE_DIR}/utilite)
    target_include_directories(${X} PRIVATE ${PROJECT_SOURCE_DIR}/lib/OpenXLSX-master/library)
    target_include_directories(${X} PRIVATE ${PROJECT_SOURCE_DIR}/lib/curl-master/include)
    target_include_directories(${X} PRIVATE ${PROJECT_SOURCE_DIR}/lib/libsqlite3-master)
    target_include_directories(${X} PRIVATE ${PROJECT_SOURCE_DIR}/lib/csv-parser-master/include)

    target_link_libraries(${X} OpenXLSX-static libcurl sqlite3 nlohmann_json csv)
    target_compile_definitions(${X} PRIVATE OPENXLSX_STATIC_DEFINE)
    message( "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||")
endforeach()


