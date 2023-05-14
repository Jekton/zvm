#pragma once

#include <array>
#include <stddef.h>

#include "loader/ConstantPool.h"
#include "util/types.h"

namespace zvm::loader {

class ClassFile {
public:
    static constexpr int kAccPublic = 0x0001;
    static constexpr int kAccFinal = 0x0010;
    static constexpr int kAccSuper = 0x0020;
    static constexpr int kAccInterface = 0x0200;
    static constexpr int kAccAbstract = 0x0400;
    static constexpr int kAccSynthetic = 0x1000;
    static constexpr int kAccAnnotation = 0x2000;
    static constexpr int kAccEnum = 0x4000;

    u4 magic() const { return mMagic; }
    u2 majorVersion() const { return mMajorVersion; }
    u2 minorVersion() const { return mMinorVersion; }
    const ConstantPool& constantPool() const { return mConstantPool; }
    u2 accessFlags() const { return mAccessFlags; }
    u2 thisClassIndex() const { return mThisClass; }
    u2 superClassIndex() const { return mSuperClass; }
    const std::string* thisClass() const {
        return mConstantPool.getClass(mThisClass);
    }
    const std::string* superClass() const {
        return mConstantPool.getClass(mSuperClass);
    }

private:
    friend class ClassFileParser;
    u4 mMagic;
    u2 mMinorVersion;
    u2 mMajorVersion;
    u2 mAccessFlags;
    u2 mThisClass;
    u2 mSuperClass;
    ConstantPool mConstantPool;
};

}