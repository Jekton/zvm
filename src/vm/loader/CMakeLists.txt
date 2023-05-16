
add_library(loader
    ClassFile.cpp
    ClassFileParser.cpp
    ClassVerifier.cpp
    ConstantPool.cpp
)

add_executable(ClassFileParserTest
    ClassFileParserTest.cpp
)
target_link_libraries(ClassFileParserTest
    loader
    util
    GTest::gtest_main
)
gtest_discover_tests(ClassFileParserTest)