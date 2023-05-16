
#include <arpa/inet.h>

#include "loader/ClassFileParser.h"
#include "loader/ClassVerifier.h"
#include "loader/ConstantPool.h"
#include "util/debug.h"

static const char* kTag = "ClassFileParser";

namespace zvm::loader {

static ClassFile::Method* parseAMethod(util::File& file, const ConstantPool& constantPool);

static void parseAttributes(
    util::File& file,
    const ConstantPool& constantPool,
    int count,
    std::map<std::string_view, const void*>& attributes);
static const void* codeParser(
    util::File& file,
    const ConstantPool& constantPool,
    u4 atrtributeLength);
static const void* sourceFileParser(
    util::File& file,
    const ConstantPool& constantPool,
    u4 atrtributeLength);

static bool verifyClass(ClassFile* classFile);

static std::map<std::string_view, decltype(&codeParser)> sAttributeParsers = {
    {ClassFile::kAttributeCode, codeParser},
    {ClassFile::kAttributeSourceFile, sourceFileParser},
};


std::unique_ptr<ClassFile> ClassFileParser::parse(util::File file) {
    auto classFile = std::make_unique<ClassFile>();
    ensure(file.readBigEndian(&classFile->mMagic), "fail to read class magic");
    ensure(file.readBigEndian(&classFile->mMinorVersion), "fail to read class version");
    ensure(file.readBigEndian(&classFile->mMajorVersion), "fail to read class version");

    u2 constantPoolCount;
    ensure(classFile->mConstantPool.parseCPInfo(file), "fail to read constant pool");

    ensure(file.readBigEndian(&classFile->mAccessFlags), "fail to read access flags");
    ensure(file.readBigEndian(&classFile->mThisClass), "fail to read this class");
    ensure(file.readBigEndian(&classFile->mSuperClass), "fail to read super class");
    u2 interfaceCount;
    ensure(file.readBigEndian(&interfaceCount), "fail to read interface count");
    ensure(interfaceCount == 0, "interface not supported yet!");

    u2 fieldCount;
    ensure(file.readBigEndian(&fieldCount), "fail to read field count");
    ensure(fieldCount == 0, "field not supported yet!");

    u2 methodCount;
    ensure(file.readBigEndian(&methodCount), "fail to read method count");
    for (int i = 0; i < methodCount; ++i) {
        auto method = parseAMethod(file, classFile->constantPool());
        classFile->mMethods.push_back(method);
    }

    u2 attributeCount;
    ensure(file.readBigEndian(&attributeCount), "fail to read attribute count of class");
    parseAttributes(file, classFile->constantPool(), attributeCount, classFile->mAttributes);

    if (!verifyClass(classFile.get())) {
        classFile.reset();
    }
    return classFile;
}

static ClassFile::Method* parseAMethod(util::File& file, const ConstantPool& constantPool) {
    auto method = new ClassFile::Method(constantPool);
    ensure(file.readBigEndian(&method->accessFlags), "fail to read access flasgs of method");
    ensure(file.readBigEndian(&method->nameIndex), "fail to read name index of method");
    ensure(file.readBigEndian(&method->descriptorIndex), "fail to read descriptor index of method");
    u2 attributeCount;
    ensure(file.readBigEndian(&attributeCount), "fail to read attribute count of method");
    parseAttributes(file, constantPool, attributeCount, method->attributes);
    return method;
}

static void parseAttributes(
    util::File& file,
    const ConstantPool& constantPool,
    int count,
    std::map<std::string_view, const void*>& attributes
) {
    for (int i = 0; i < count; ++i) {
        u2 nameIndex;
        ensure(file.readBigEndian(&nameIndex), "fail to read attribute name index");
        auto name = constantPool.getUtf8(nameIndex);
        LOGD(kTag, "name = %s", name->c_str());
        u4 len;
        ensure(file.readBigEndian(&len), "fail to read attribute length");
        auto parser = sAttributeParsers[name->c_str()];
        if (!parser) {
            LOGW(kTag, "attribute %s skipped!", name->c_str());
            auto buf = (u1*) alloca(len);
            ensure(file.read(buf, len), "fail to read attribute info");
        } else {
            auto attribute = parser(file, constantPool, len);
            attributes[name->c_str()] = attribute;
        }
    }
}

static const void* codeParser(
    util::File& file,
    const ConstantPool& constantPool,
    u4 atrtributeLength
) {
    auto code = new ClassFile::Code;
    ensure(file.readBigEndian(&code->maxStack), "fail to read max stack");
    ensure(file.readBigEndian(&code->maxLocals), "fail to read max locals");
    u4 codeLength;
    ensure(file.readBigEndian(&codeLength), "fail to read code length");
    auto instructions = new u1[codeLength];
    code->instructions = instructions;
    ensure(file.read(instructions, codeLength), "fail to read codes");
    code->codeLength = codeLength;

    u2 exceptionTableLength;
    ensure(file.readBigEndian(&exceptionTableLength), "fail to read exception table length");
    ensure(exceptionTableLength == 0, "exception not supported yet!");

    u2 attributeCount;
    ensure(file.readBigEndian(&attributeCount), "fail to read attribute count of code");
    parseAttributes(file, constantPool, attributeCount, code->attributes);

    return code;
}

static const void* sourceFileParser(
    util::File& file,
    const ConstantPool& constantPool,
    u4 atrtributeLength
) {
    u2 index;
    ensure(file.readBigEndian(&index), "fail to read index of SourceFile attribute");
    return constantPool.getUtf8(index);
}


static bool verifyClass(ClassFile* classFile) {
    ClassVerifier verifier(classFile);
    return verifier.verify();
}

}

