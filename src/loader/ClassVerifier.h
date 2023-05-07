
#pragma once

#include "loader/ClassFile.h"

namespace zvm::loader {

class ClassVerifier {
public:
    ClassVerifier(ClassFile* classFile): mClassFile(classFile) { }
    bool verify() const;

private:
    ClassFile* mClassFile;
};

}