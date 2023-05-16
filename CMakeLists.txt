cmake_minimum_required(VERSION 3.14)
project(zvm VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/b796f7d44681514f58a683a3a71ff17c94edb0c1.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
include(GoogleTest)

configure_file(zvm_config.h.in  zvm_config.h)

include_directories(PUCLIC src/vm)
include_directories(PRIVATE ${PROJECT_BINARY_DIR})


add_subdirectory(src/vm/dumper)
add_subdirectory(src/vm/loader)
add_subdirectory(src/vm/util)

add_executable(zvm
    src/vm/main.cpp
)

target_link_libraries(zvm loader dumper util)

