cmake_minimum_required(VERSION 3.10)
project(zvm VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

configure_file(zvm_config.h.in  zvm_config.h)

include_directories(PUCLIC src)
include_directories(PRIVATE ${PROJECT_BINARY_DIR})

add_subdirectory(src/dumper)
add_subdirectory(src/loader)
add_subdirectory(src/util)

add_executable(zvm
    src/main.cpp
)

target_link_libraries(zvm loader dumper util)

