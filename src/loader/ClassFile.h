#pragma once

#include <array>
#include <stddef.h>

#include "util/types.h"

namespace zvm::loader {

class ClassFile {
public:
    u4 magic() const { return mMagic; }
    u2 majorVersion() const { return mMajorVersion; }
    u2 minorVersion() const { return mMinorVersion; }

private:
    friend class ClassFileParser;
    u4 mMagic;
    u2 mMinorVersion;
    u2 mMajorVersion;
};

}