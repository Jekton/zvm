
#include <gtest/gtest.h>

#include <string>

#include "loader/ClassFileParser.h"
#include "util/File.h"
#include "zvm_config.h"

using namespace zvm;

class ClassFileParserTest : public testing::Test {
protected:
    void SetUp() override {
        auto path = std::string(kSourceDir) + "/test/HelloWorld.class";
        auto file = util::openFile(path.c_str(), "r");
        auto classFile = loader::ClassFileParser::parse(std::move(file));
        mClassFile = std::move(classFile);
    }

    void TearDown() override { }

    std::unique_ptr<loader::ClassFile> mClassFile;
};

TEST_F(ClassFileParserTest, Precondition) {
    ASSERT_NE(mClassFile.get(), nullptr);
}

TEST_F(ClassFileParserTest, Magic) {
    ASSERT_EQ(mClassFile->magic(), 0xcafebabe);
}

TEST_F(ClassFileParserTest, Version) {
    ASSERT_EQ(mClassFile->majorVersion(), 61);
    ASSERT_EQ(mClassFile->minorVersion(), 0);
}