#pragma once

#include <array>
#include <stddef.h>

#include "util/types.h"

namespace zvm::loader {

class ClassFile {
public:
    u4 magic() const { return mMagic; }

private:
    friend class ClassFileParser;
    u4 mMagic;
};

}