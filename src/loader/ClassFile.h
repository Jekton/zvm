#pragma once

#include <array>
#include <stddef.h>

#include "util/types.h"

namespace zvm {

namespace loader {
    class ClassFileParser;
}

class ClassFile {
public:
    u4 magic() const { return mMagic; }

private:
    friend class loader::ClassFileParser;
    u4 mMagic;
};

}