
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
        auto file = util::File(path.c_str(), "r");
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

TEST_F(ClassFileParserTest, ConstantPollCount) {
    auto& pool = mClassFile->constantPool();
    ASSERT_EQ(pool.count(), 29);
}

TEST_F(ClassFileParserTest, ConstantPoll) {
    auto& pool = mClassFile->constantPool();
    ASSERT_EQ(pool[1]->tag, loader::CPInfo::kTagMethodRef);
    ASSERT_EQ(pool[2]->tag, loader::CPInfo::kTagClass);
    ASSERT_EQ(pool[3]->tag, loader::CPInfo::kTagNameAndType);
    ASSERT_EQ(pool[4]->tag, loader::CPInfo::kTagUtf8);
    ASSERT_EQ(pool[5]->tag, loader::CPInfo::kTagUtf8);
    ASSERT_EQ(pool[6]->tag, loader::CPInfo::kTagUtf8);
    ASSERT_EQ(pool[7]->tag, loader::CPInfo::kTagFieldRef);
    ASSERT_EQ(pool[8]->tag, loader::CPInfo::kTagClass);
    ASSERT_EQ(pool[9]->tag, loader::CPInfo::kTagNameAndType);
    ASSERT_EQ(pool[10]->tag, loader::CPInfo::kTagUtf8);
    ASSERT_EQ(pool[11]->tag, loader::CPInfo::kTagUtf8);
    ASSERT_EQ(pool[12]->tag, loader::CPInfo::kTagUtf8);
    ASSERT_EQ(pool[13]->tag, loader::CPInfo::kTagString);
    ASSERT_EQ(pool[14]->tag, loader::CPInfo::kTagUtf8);
    ASSERT_EQ(pool[15]->tag, loader::CPInfo::kTagMethodRef);
    ASSERT_EQ(pool[16]->tag, loader::CPInfo::kTagClass);
    ASSERT_EQ(pool[17]->tag, loader::CPInfo::kTagNameAndType);
    ASSERT_EQ(pool[18]->tag, loader::CPInfo::kTagUtf8);
    ASSERT_EQ(pool[19]->tag, loader::CPInfo::kTagUtf8);
    ASSERT_EQ(pool[20]->tag, loader::CPInfo::kTagUtf8);
    ASSERT_EQ(pool[21]->tag, loader::CPInfo::kTagClass);
    ASSERT_EQ(pool[22]->tag, loader::CPInfo::kTagUtf8);
    ASSERT_EQ(pool[23]->tag, loader::CPInfo::kTagUtf8);
    ASSERT_EQ(pool[24]->tag, loader::CPInfo::kTagUtf8);
    ASSERT_EQ(pool[25]->tag, loader::CPInfo::kTagUtf8);
    ASSERT_EQ(pool[26]->tag, loader::CPInfo::kTagUtf8);
    ASSERT_EQ(pool[27]->tag, loader::CPInfo::kTagUtf8);
    ASSERT_EQ(pool[28]->tag, loader::CPInfo::kTagUtf8);
}

TEST_F(ClassFileParserTest, FlagsAndClass) {
    ASSERT_EQ(mClassFile->accessFlags(), 0x0020);
    ASSERT_EQ(mClassFile->thisClassIndex(), 21);
    ASSERT_EQ(mClassFile->superClassIndex(), 2);
    ASSERT_STREQ(mClassFile->thisClass()->c_str(), "test/HelloWorld");
    ASSERT_STREQ(mClassFile->superClass()->c_str(), "java/lang/Object");
}