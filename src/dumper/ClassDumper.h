#pragma once

#include <stdio.h>

#include "loader/ClassFile.h"

namespace zvm::dumper {

class ClassDumper {
public:
    ClassDumper(std::unique_ptr<loader::ClassFile>&& classFile)
        : mClassFile(std::move(classFile)) { }
    void dump(FILE* out) const;

private:
    void dumpConstantPool(FILE* out) const;
    void dumpAccessFlags(FILE* out) const;

    std::unique_ptr<loader::ClassFile> mClassFile;
};

}