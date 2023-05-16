
#include <gtest/gtest.h>

#include <string>

#include "loader/ClassFileParser.h"
#include "util/File.h"
#include "zvm_config.h"

using namespace zvm;


TEST(ClassFileParserTest, HelloWorld0) {
    auto path = std::string(kSourceDir) + "/test/HelloWorld.class";
    auto file = util::File(path.c_str(), "r");
    auto classFile = loader::ClassFileParser::parse(std::move(file));

    ASSERT_NE(classFile.get(), nullptr);
    ASSERT_EQ(classFile->magic(), 0xcafebabe);
    ASSERT_EQ(classFile->majorVersion(), 61);
    ASSERT_EQ(classFile->minorVersion(), 0);

    auto& pool = classFile->constantPool();
    ASSERT_EQ(pool.count(), 29);
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

    ASSERT_EQ(classFile->accessFlags(), 0x0020);
    ASSERT_EQ(classFile->thisClassIndex(), 21);
    ASSERT_EQ(classFile->superClassIndex(), 2);
    ASSERT_STREQ(classFile->thisClass()->c_str(), "test/HelloWorld");
    ASSERT_STREQ(classFile->superClass()->c_str(), "java/lang/Object");

    auto interfaces = classFile->interfaces();
    ASSERT_EQ(interfaces.size(), 0);

    auto fields = classFile->fields();
    ASSERT_EQ(fields.size(), 0);

    auto methods = classFile->methods();
    ASSERT_EQ(methods.size(), 2);
    auto method = methods[0];
    ASSERT_STREQ(method->name()->c_str(), "<init>");
    ASSERT_STREQ(method->descriptor()->c_str(), "()V");

    method = methods[1];
    ASSERT_STREQ(method->name()->c_str(), "main");
    ASSERT_STREQ(method->descriptor()->c_str(), "([Ljava/lang/String;)V");
    auto code = method->getCode();
    ASSERT_NE(code, nullptr);
    ASSERT_EQ(code->codeLength, 9);

    auto sourceFile = classFile->sourceFile();
    ASSERT_NE(sourceFile, nullptr);
    ASSERT_STREQ(sourceFile->c_str(), "HelloWorld.java");
}


TEST(ClassFileParserTest, String) {
    auto path = std::string(kSourceDir) + "/src/zvm.base/z/lang/String.class";
    auto file = util::File(path.c_str(), "r");
    auto classFile = loader::ClassFileParser::parse(std::move(file));

    ASSERT_NE(classFile.get(), nullptr);
    ASSERT_EQ(classFile->magic(), 0xcafebabe);
    ASSERT_EQ(classFile->majorVersion(), 61);
    ASSERT_EQ(classFile->minorVersion(), 0);

    auto& pool = classFile->constantPool();
    ASSERT_EQ(pool.count(), 20);
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
    ASSERT_EQ(pool[13]->tag, loader::CPInfo::kTagUtf8);
    ASSERT_EQ(pool[14]->tag, loader::CPInfo::kTagUtf8);
    ASSERT_EQ(pool[15]->tag, loader::CPInfo::kTagUtf8);
    ASSERT_EQ(pool[16]->tag, loader::CPInfo::kTagUtf8);
    ASSERT_EQ(pool[17]->tag, loader::CPInfo::kTagUtf8);
    ASSERT_EQ(pool[18]->tag, loader::CPInfo::kTagUtf8);
    ASSERT_EQ(pool[19]->tag, loader::CPInfo::kTagUtf8);

    ASSERT_EQ(classFile->accessFlags(), 0x0021);
    ASSERT_EQ(classFile->thisClassIndex(), 8);
    ASSERT_EQ(classFile->superClassIndex(), 2);
    ASSERT_STREQ(classFile->thisClass()->c_str(), "z/lang/String");
    ASSERT_STREQ(classFile->superClass()->c_str(), "java/lang/Object");

    auto interfaces = classFile->interfaces();
    ASSERT_EQ(interfaces.size(), 0);

    auto fields = classFile->fields();
    ASSERT_EQ(fields.size(), 1);
    auto field = fields[0];
    ASSERT_STREQ(field->name()->c_str(), "chars");
    ASSERT_STREQ(field->descriptor()->c_str(), "J");

    auto methods = classFile->methods();
    ASSERT_EQ(methods.size(), 2);

    auto method = methods[0];
    ASSERT_STREQ(method->name()->c_str(), "<init>");
    ASSERT_STREQ(method->descriptor()->c_str(), "(J)V");

    method = methods[1];
    ASSERT_STREQ(method->name()->c_str(), "getChars");
    ASSERT_STREQ(method->descriptor()->c_str(), "()J");
    auto code = method->getCode();
    ASSERT_NE(code, nullptr);
    ASSERT_EQ(code->codeLength, 5);

    auto sourceFile = classFile->sourceFile();
    ASSERT_NE(sourceFile, nullptr);
    ASSERT_STREQ(sourceFile->c_str(), "String.java");
}