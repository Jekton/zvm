
#include <arpa/inet.h>

#include "loader/ClassFileParser.h"
#include "loader/ClassVerifier.h"
#include "loader/ConstantPool.h"
#include "util/debug.h"

namespace zvm::loader {

static bool verifyClass(ClassFile* classFile);


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


    if (!verifyClass(classFile.get())) {
        classFile.reset();
    }
    return classFile;
}


static bool verifyClass(ClassFile* classFile) {
    ClassVerifier verifier(classFile);
    return verifier.verify();
}

}

