#pragma once

#include <stdio.h>

#include "loader/ClassFile.h"

namespace zvm {

class ClassDumper {
public:
    ClassDumper(std::unique_ptr<ClassFile>&& classFile)
        : mClassFile(std::move(classFile)) { }
    void dump(FILE* out) const;

private:
    std::unique_ptr<ClassFile> mClassFile;
};

}