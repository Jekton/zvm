cmake_minimum_required(VERSION 3.10)
project(zvm VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

configure_file(zvm_config.h.in  zvm_config.h)

add_executable(zvm
    src/main.cpp
)
target_include_directories(zvm
    PRIVATE ${PROJECT_BINARY_DIR}
)